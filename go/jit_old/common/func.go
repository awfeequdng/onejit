/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * func.go
 *
 *  Created on Feb 07, 2019
 *      Author Massimiliano Ghilardi
 */

package common

import (
	"reflect"
	"unsafe"
)

const MMAP_VERBOSE = false

type interfaceHeader struct {
	typ  uintptr
	addr **MemArea
}

/**
 * convert code created by the programmer to a callable function.
 * funcaddr must be a non-nil pointer to function.
 *
 * function type MUST match the code created by the programmer,
 * or BAD things will happen: crash, memory corruption, undefined behaviour...
 *
 * Obviously, code created by the programmer must be for the same architecture
 * the program is currently running on...
 *
 * implemented as Asm.Mmap() + Asm.MemToFunc()
 */
func (asm *Asm) Func(funcaddr interface{}) {
	checkFuncAddr(funcaddr)
	mem := asm.Mmap()
	asm.MemToFunc(funcaddr, mem)
}

/**
 * convert code created by the programmer to a callable function.
 *
 * funcaddr must be a non-nil pointer to function,
 * and area must have been returned by Asm.Mmap()
 *
 * function type MUST match the code created by the programmer,
 * or BAD things will happen: crash, memory corruption, undefined behaviour...
 *
 * Obviously, code created by the programmer must be for the same architecture
 * the program is currently running on...
 *
 * used to implement Asm.Func()
 */
func (asm *Asm) MemToFunc(funcaddr interface{}, mem MemArea) {
	checkFuncAddr(funcaddr)
	header := *(*interfaceHeader)(unsafe.Pointer(&funcaddr))
	*header.addr = &mem
}

func checkFuncAddr(funcaddr interface{}) {
	v := reflect.ValueOf(funcaddr)
	if !v.IsValid() || v.Kind() != reflect.Ptr || v.IsNil() || !v.Elem().CanSet() || v.Elem().Kind() != reflect.Func {
		errorf("Asm.Func() argument must be non-nil, settable pointer to function, received %p // %T", funcaddr, funcaddr)
	}
}

// return a MemArea with executable machine code equal to asm.Code().
// Also calls asm.link()
func (asm *Asm) Mmap() MemArea {
	asm.Epilogue()
	if MMAP_VERBOSE {
		debugf("asm: %#v", asm.code)
	}
	area := asm.code.MemArea()
	size := area.Size()
	// cache lookups are ruined by absolute jumps :(
	if len(asm.jump) == 0 {
		if ret := asm.cache.Lookup(area); ret.Size() == size {
			return ret
		}
	}
	if asm.pool.Size() < size {
		// we waste asm.mem.Size() bytes of mmapped memory...
		asm.pool = NewMemPool(size)
	}
	asm.Link(uintptr(unsafe.Pointer(asm.pool.Addr())))
	ret := asm.pool.Copy(area)
	if asm.cache == nil {
		asm.cache = make(Cache)
	}
	asm.cache.Add(ret)
	return ret
}

// now that final destination of machine code is known,
// fill jumps to absolute destinations
func (asm *Asm) Link(address uintptr) {
	code := asm.code.Bytes
	for index, dst := range asm.jump {
		// JMP 0 means jump to next instruction i.e. NOP,
		// so JMP offsets are computed from the end
		// of the 32 bit JMP offset itself
		src := address + uintptr(index) + 4

		delta := dst - src
		idelta := int32(delta)
		if delta != uintptr(idelta) {
			errorf("absolute JMP 0x%x is too far from 0x%x: offset does not fit int32", dst, src)
		}
		// FIXME this works only for AMD64
		code[index] = uint8(idelta)
		code[index+1] = uint8(idelta >> 8)
		code[index+2] = uint8(idelta >> 16)
		code[index+3] = uint8(idelta >> 24)
	}
	asm.jump = nil
}

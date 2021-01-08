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
 * api.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package common

type Size uint8 // 1, 2, 4 or 8

// symbolic assembly code: instruction or its arguments
type AsmCode interface {
	asmcode()
}

// assembled machine code.
// Executable if compiled for the same architecture
// the program is running on - see Asm.Func()
type MachineCode struct {
	ArchId ArchId
	Bytes  []uint8
}

// argument of assembly instructions
type Arg interface {
	RegId() RegId // register used by Arg, or NoReg if Arg is Const
	Kind() Kind
	Const() bool
	asmcode()
}

// subset of Arg interface
type Expr interface {
	Kind() Kind
	Const() bool
}

// memory area where spill registers can be saved
type Save struct {
	reg              Reg      // points to memory area
	start, next, end SaveSlot // memory area indexes
	bitmap           []bool   // bitmap of used/free indexes
}

func SizeOf(e Expr) Size {
	size := e.Kind().Size()
	if size == 0 {
		errorf("unknown kind: %v", e.Kind())
	}
	return size
}

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
 * kind.go
 *
 *  Created on Jan 24, 2019
 *      Author Massimiliano Ghilardi
 */

package common

import (
	"reflect"
)

type Kind uint8 // narrow version of reflect.Kind

const (
	Invalid = Kind(reflect.Invalid)
	Bool    = Kind(reflect.Bool)
	Int     = Kind(reflect.Int)
	Int8    = Kind(reflect.Int8)
	Int16   = Kind(reflect.Int16)
	Int32   = Kind(reflect.Int32)
	Int64   = Kind(reflect.Int64)
	Uint    = Kind(reflect.Uint)
	Uint8   = Kind(reflect.Uint8)
	Uint16  = Kind(reflect.Uint16)
	Uint32  = Kind(reflect.Uint32)
	Uint64  = Kind(reflect.Uint64)
	Uintptr = Kind(reflect.Uintptr)
	Float32 = Kind(reflect.Float32)
	Float64 = Kind(reflect.Float64)
	Ptr     = Kind(reflect.Ptr)
	KLo     = Bool
	KHi     = Ptr
)

var ksize = [...]Size{
	Bool:    1,
	Int:     Size(reflect.TypeOf(int(0)).Size()),
	Int8:    1,
	Int16:   2,
	Int32:   4,
	Int64:   8,
	Uint:    Size(reflect.TypeOf(uint(0)).Size()),
	Uint8:   1,
	Uint16:  2,
	Uint32:  4,
	Uint64:  8,
	Uintptr: Size(reflect.TypeOf(uintptr(0)).Size()),
	Float32: 4,
	Float64: 8,
	Ptr:     Size(reflect.TypeOf((*int)(nil)).Size()),
}

func (k Kind) Size() Size {
	if k >= KLo && k <= KHi {
		return ksize[k]
	}
	return 0
}

func (k Kind) Signed() bool {
	switch k {
	case Bool, Uint, Uint8, Uint16, Uint32, Uint64, Uintptr, Ptr:
		return false
	default:
		return true
	}
}

func (k Kind) IsFloat() bool {
	return k == Float32 || k == Float64
}

func (k Kind) String() string {
	return reflect.Kind(k).String()
}

// implement AsmCode interface
func (k Kind) asmcode() {
}

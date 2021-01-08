/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
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

package jit

import (
	"github.com/cosmos72/onejit/go/jit/internal"
)

const (
	CpuWidth32   = internal.CpuWidth32
	CpuWidth64   = internal.CpuWidth64
	CpuWidthHost = internal.CpuWidthHost
)

func GetCpuWidth() CpuWidth {
	return internal.GetCpuWidth()
}

// if host and target have different CPU widths (32 vs 64 bit),
// call this function *BEFORE* compiling, specifying target CPU width
func SetCpuWidth(val CpuWidth) {
	internal.SetCpuWidth(val)
}

// intentionally matches reflect.Kind values
const (
	Void       = internal.Void // Invalid
	Bool       = internal.Bool
	Int        = internal.Int
	Int8       = internal.Int8
	Int16      = internal.Int16
	Int32      = internal.Int32
	Int64      = internal.Int64
	Uint       = internal.Uint
	Uint8      = internal.Uint8
	Uint16     = internal.Uint16
	Uint32     = internal.Uint32
	Uint64     = internal.Uint64
	Uintptr    = internal.Uintptr
	Float32    = internal.Float32
	Float64    = internal.Float64
	Complex64  = internal.Complex64
	Complex128 = internal.Complex128
	_          // Array
	_          // Chan
	_          // Func
	_          // Interface
	_          // Map
	Ptr        = internal.Ptr
	_          // Slice
	_          // String
	_          // Struct
	_          // UnsafePointer
	ArchFlags  = internal.ArchFlags
)

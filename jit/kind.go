/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * kind.go
 *
 *  Created on Jan 24, 2019
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/onejit/jit/internal"
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

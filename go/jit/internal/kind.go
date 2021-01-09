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

package internal

import (
	"strconv"
)

type CpuWidth uint8 // 32 or 64

const (
	CpuWidth32 = CpuWidth(32)
	CpuWidth64 = CpuWidth(64)
	// only works for 32 or 64 bits
	CpuWidthHost = CpuWidth(((^uintptr(0) >> 31) & 32) + 32)
)

var cpuWidth CpuWidth = CpuWidthHost

func GetCpuWidth() CpuWidth {
	return cpuWidth
}

func SetCpuWidth(val CpuWidth) {
	if val != CpuWidth32 && val != CpuWidth64 {
		Errorf("bad CPUWidth: have %d, want 32 or 64", int(val))
	}
	cpuWidth = val

	ksize[Int] = Size(val / 8)
	ksize[Uint] = Size(val / 8)
	ksize[Uintptr] = Size(val / 8)
	ksize[Ptr] = Size(val / 8)

	k := Int64
	if val == CpuWidth32 {
		k = Int32
	}
	ksuffix[Int] = ksuffix[k]

	k = Uint64
	if val == CpuWidth32 {
		k = Uint32
	}
	ksuffix[Uint] = ksuffix[k]

	k = Uint64
	if val == CpuWidth32 {
		k = Uint32
	}
	ksuffix[Uintptr] = ksuffix[k]
}

func init() {
	SetCpuWidth(CpuWidthHost)
}

type Size uint64 // 1, 2, 4, 8 or 16

// intentionally match reflect.Kind values
type Kind uint8

const (
	Void Kind = iota // Invalid
	Bool
	Int
	Int8
	Int16
	Int32
	Int64
	Uint
	Uint8
	Uint16
	Uint32
	Uint64
	Uintptr
	Float32
	Float64
	Complex64
	Complex128
	_ // Array
	_ // Chan
	_ // Func
	_ // Interface
	_ // Map
	Ptr
	_ // Slice
	_ // String
	_ // Struct
	_ // UnsafePointer
	ArchFlags

	kLo = Void
	kHi = ArchFlags
)

var kstring = [kHi + 1]string{
	Void:       "void",
	Bool:       "bool",
	Int:        "int",
	Int8:       "int8",
	Int16:      "int16",
	Int32:      "int32",
	Int64:      "int64",
	Uint:       "uint",
	Uint8:      "uint8",
	Uint16:     "uint16",
	Uint32:     "uint32",
	Uint64:     "uint64",
	Uintptr:    "uintptr",
	Float32:    "float32",
	Float64:    "float64",
	Complex64:  "complex64",
	Complex128: "complex128",
	/*
		Array:         "array",
		Chan:          "chan",
		Func:          "func",
		Interface:     "interface",
		Map:           "map",
	*/
	Ptr: "ptr",
	/*
		Slice:         "slice",
		String:        "string",
		Struct:        "struct",
		UnsafePointer: "unsafe.Pointer",
	*/
	ArchFlags: "ArchFlags",
}

var ksize = [kHi + 1]Size{
	Bool:       1,
	Int8:       1,
	Int16:      2,
	Int32:      4,
	Int64:      8,
	Uint8:      1,
	Uint16:     2,
	Uint32:     4,
	Uint64:     8,
	Float32:    4,
	Float64:    8,
	Complex64:  8,
	Complex128: 16,
	/*
		Chan:          Size(unsafe.Sizeof(make(chan int))),
		Func:          Size(unsafe.Sizeof(func() {})),
		Interface:     Size(unsafe.Sizeof(assertError)),
		Map:           Size(unsafe.Sizeof(map[int]int{})),
	*/
	/*
		Slice:         Size(unsafe.Sizeof([]int{})),
		String:        Size(unsafe.Sizeof("")),
		UnsafePointer: Size(unsafe.Sizeof((unsafe.Pointer)(nil))),
	*/
}

func (k Kind) Size() Size {
	if k >= kLo && k <= kHi {
		return ksize[k]
	}
	return 0
}

func (k Kind) String() string {
	var s string
	if k >= kLo && k <= kHi {
		s = kstring[k]
	} else {
		s = "Kind(" + strconv.Itoa(int(k)) + ")"
	}
	return s
}

var ksuffix = [kHi + 1]string{
	Bool:       "e",
	Int8:       "b",
	Int16:      "h",
	Int32:      "i",
	Int64:      "l",
	Uint8:      "ub",
	Uint16:     "uh",
	Uint32:     "ui",
	Uint64:     "ul",
	Float32:    "f",
	Float64:    "lf",
	Complex64:  "c",
	Complex128: "lc",
	Ptr:        "p",
	ArchFlags:  "fl",
}

func (k Kind) SizeString() string {
	var s string
	if k >= kLo && k <= kHi {
		s = ksuffix[k]
	}
	return s
}

func (k Kind) Category() Kind {
	switch k {
	case Int, Int8, Int16, Int32, Int64:
		k = Int
	case Uint, Uint8, Uint16, Uint32, Uint64, Uintptr:
		k = Uint
	case Float32, Float64:
		k = Float64
	case Complex64, Complex128:
		k = Complex128
	}
	return k
}

func (k Kind) IsSigned() bool {
	var ret bool
	switch k {
	case Int, Int8, Int16, Int32, Int64, Float32, Float64, Complex64, Complex128:
		ret = true
	}
	return ret
}

func (k Kind) IsUnsigned() bool {
	var ret bool
	switch k {
	case Uint, Uint8, Uint16, Uint32, Uint64, Uintptr, Ptr:
		ret = true
	}
	return ret
}

func (k Kind) IsInteger() bool {
	var ret bool
	switch k {
	case Int, Int8, Int16, Int32, Int64, Uint, Uint8, Uint16, Uint32, Uint64, Uintptr:
		ret = true
	}
	return ret
}

func (k Kind) IsIntegerOrPtr() bool {
	return k == Ptr || k.IsInteger()
}

func (k Kind) IsFloat() bool {
	return k == Float32 || k == Float64
}

func (k Kind) IsComplex() bool {
	return k == Complex64 || k == Complex128
}

func (k Kind) IsOrdered() bool {
	return k.IsIntegerOrPtr() || k.IsFloat()
}

func (k Kind) IsComparable() bool {
	return k.IsIntegerOrPtr() || k.IsFloat() || k.IsComplex()
}

func (k Kind) mustBe(expected Kind, op Op) Kind {
	if k != expected {
		BadOpKind(op, k)
	}
	return k
}

func (k Kind) mustBeIntegerOrPtr(op Op) Kind {
	if !k.IsIntegerOrPtr() {
		BadOpKind(op, k)
	}
	return k
}

func KindMustBe(op Op, k Kind, expected Kind) Kind {
	return k.mustBe(expected, op)
}

func KindMustBeIntegerOrPtr(op Op, k Kind) Kind {
	return k.mustBeIntegerOrPtr(op)
}

func KindMustBeNumberOrPtr(op Op, k Kind) Kind {
	if k == Bool {
		BadOpKind(op, k)
	}
	return k
}

func Kind2MustBeIntegerOrPtr(op Op, k1 Kind, k2 Kind) Kind {
	if k1 == k2 && k1.IsIntegerOrPtr() {
		return k1
	}
	return BadOpKind2(op, k1, k2)
}

func Kind2MustBeOrdered(op Op, k1 Kind, k2 Kind) Kind {
	if k1 == k2 && k1.IsOrdered() {
		return k1
	}
	return BadOpKind2(op, k1, k2)
}

func Kind2MustBeComparable(op Op, k1 Kind, k2 Kind) Kind {
	if k1 == k2 && k1.IsComparable() {
		return k1
	}
	return BadOpKind2(op, k1, k2)
}

func Kind2MustBeNumberOrPtr(op Op, k1 Kind, k2 Kind) Kind {
	if k1 == k2 && k1 != Bool {
		return k1
	}
	return BadOpKind2(op, k1, k2)
}

func KindMustBeUintptrOrPtr(label string, k Kind) Kind {
	switch k {
	case Uintptr, Ptr:
		break
	default:
		Errorf("bad %s address kind: have %v, want UintPtr or Ptr", label, k)
	}
	return k
}

func KindMustBeSubset(label string, subset Kind, whole Kind) {
	if whole.Size() >= subset.Size() {
		if whole.IsIntegerOrPtr() && subset.IsIntegerOrPtr() {
			return
		}
		// FLOAT subset of a COMPLEX register is the real part
		if whole.IsComplex() && subset.IsFloat() && whole.Size() == subset.Size()*2 {
			return
		}
	}
	Errorf("invalid %s.ReadOnly: %v is not a subset of %v", label, subset, whole)
}

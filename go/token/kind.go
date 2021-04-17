/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * kind.go
 *
 *  Created on: Mar 31, 2021
 *      Author: Massimiliano Ghilardi
 */

package token

import "github.com/cosmos72/onejit/go/arch"

// equivalent to go/token.Kind
type Kind uint8

const (
	Invalid Kind = iota

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
	ArrayKind
	ChanKind
	FuncKind
	InterfaceKind
	MapKind
	PtrKind
	SliceKind
	String
	StructKind
	UnsafePointer

	// types for untyped values
	UntypedBool
	UntypedInt
	UntypedRune
	UntypedFloat
	UntypedComplex
	UntypedString
	UntypedNil

	// aliases
	Byte = Uint8
	Rune = Int32
)

var kstring = [...]string{
	Invalid:        "invalid", // also used by Builtin
	Bool:           "bool",
	Int:            "int",
	Int8:           "int8",
	Int16:          "int16",
	Int32:          "int32",
	Int64:          "int64",
	Uint:           "uint",
	Uint8:          "uint8",
	Uint16:         "uint16",
	Uint32:         "uint32",
	Uint64:         "uint64",
	Uintptr:        "uintptr",
	Float32:        "float32",
	Float64:        "float64",
	Complex64:      "complex64",
	Complex128:     "complex128",
	ArrayKind:      "array",
	ChanKind:       "chan",
	FuncKind:       "func",
	InterfaceKind:  "interface",
	MapKind:        "map",
	PtrKind:        "ptr",
	SliceKind:      "slice",
	String:         "string",
	StructKind:     "struct",
	UnsafePointer:  "unsafe.Pointer",
	UntypedBool:    "untyped.Bool",
	UntypedInt:     "untyped.Int",
	UntypedRune:    "untyped.Rune",
	UntypedFloat:   "untyped.Float",
	UntypedComplex: "untyped.Complex",
	UntypedString:  "untyped.String",
	UntypedNil:     "untyped.Nil",
}

var ksize = [...]int8{
	Invalid:       0,
	Bool:          1,
	Int:           -1,
	Int8:          1,
	Int16:         2,
	Int32:         4,
	Int64:         8,
	Uint:          -1,
	Uint8:         1,
	Uint16:        2,
	Uint32:        4,
	Uint64:        8,
	Uintptr:       -1,
	Float32:       4,
	Float64:       8,
	Complex64:     8,
	Complex128:    16,
	ArrayKind:     0,
	ChanKind:      -1,
	FuncKind:      -1,
	InterfaceKind: -2,
	MapKind:       -1,
	PtrKind:       -1,
	SliceKind:     -3,
	String:        -2,
	StructKind:    0,
	UnsafePointer: -1,
}

func (k Kind) String() string {
	if int(k) >= len(kstring) {
		return "???"
	}
	return kstring[k]
}

// return size of specified kind, in bytes. 0 means unknown/variable.
// returned size depends on arch.TargetArch()
func (k Kind) Size() uint64 {
	if int(k) >= len(ksize) {
		return 0
	}
	isize := int64(ksize[k])
	size := uint64(isize)
	if isize < 0 {
		size = uint64(-isize) * arch.TargetArch().Bytes()
	}
	return size
}

func (k Kind) IsBasic() bool {
	return k >= Bool && k <= Complex128 || k == String || k >= UnsafePointer && k <= UntypedNil
}

func (k Kind) IsInteger() bool {
	return (k >= Int && k <= Uintptr) || k == UntypedInt
}

func (k Kind) IsNillable() bool {
	return k >= ChanKind && k <= SliceKind || k == UnsafePointer || k == UntypedNil
}

func (k Kind) IsOrdered() bool {
	return k >= Int && k <= Float64 || k == String || k >= UntypedInt && k <= UntypedFloat || k == UntypedString
}

func (k Kind) IsUntyped() bool {
	return k >= UntypedBool
}

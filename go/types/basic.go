/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * named.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"github.com/cosmos72/onejit/go/arch"
	"github.com/cosmos72/onejit/go/io"
)

// Basic represents one of Go's predefined basic types
type Basic struct {
	basicTag struct{} // occupies zero bytes
	rtype    Complete
}

// *Basic implements Type

func (t *Basic) String() string {
	_ = t.basicTag
	return t.rtype.str
}

func (t *Basic) Underlying() Type {
	return t
}

func (t *Basic) common() *Complete {
	return &t.rtype
}

func (t *Basic) complete() {
	// nothing to do
}

func (t *Basic) WriteTo(dst io.StringWriter, flag verbose) {
	dst.WriteString(t.rtype.str)
}

// *Basic specific methods

func (t *Basic) Complete() *Complete {
	return &t.rtype
}

func (t *Basic) Kind() Kind {
	return t.rtype.kind
}

func (t *Basic) Name() string {
	return t.rtype.str
}

func (t *Basic) Size() uint64 {
	return t.rtype.size
}

var (
	basicTypes0        = makeBasicTypes0()
	basicTypesLinux386 = makeBasicTypes(basicTypes0, 4, 4)
	basicTypesOther32  = makeBasicTypes(basicTypes0, 4, 8)
	basicTypes64       = makeBasicTypes(basicTypes0, 8, 8)
)

func sizeOfPtr() uint64 {
	return arch.TargetArch().Bytes()
}

// return basic types for current arch.Target()
func basicTypes() []*Complete {
	var v []*Complete
	os, arc := arch.Target()
	if arc.Bits() > 32 {
		v = basicTypes64
	} else if os == arch.Linux && arc == arch.I386 {
		v = basicTypesLinux386
	} else {
		v = basicTypesOther32
	}
	return v
}

// return basic type for specified Kind and current GetArchSizeBits() and GetTargetOs()
func BasicType(kind Kind) *Complete {
	return basicTypes()[kind]
}

// create and return a slice containing basic types for current GetArchSizeBits() and GetTargetOs()
// use Kind as index in returned slice.
func BasicTypes() []*Complete {
	return append([]*Complete(nil), basicTypes()...)
}

// create basic types that do not depend on OS or architecture
func makeBasicTypes0() []*Complete {
	return []*Complete{
		Bool:    newBasic(Bool, 1, 1),
		Int8:    newBasic(Int8, 1, 1),
		Int16:   newBasic(Int16, 2, 2),
		Int32:   newBasic(Int32, 4, 4),
		Uint8:   newBasic(Uint8, 1, 1),
		Uint16:  newBasic(Uint16, 2, 2),
		Uint32:  newBasic(Uint32, 4, 4),
		Float32: newBasic(Float32, 4, 4),
		// the following have .Type() == nil as they cannot be used as components in new types
		UntypedBool:    newBasic(UntypedBool, unknownSize, unknownAlign),
		UntypedInt:     newBasic(UntypedInt, unknownSize, unknownAlign),
		UntypedRune:    newBasic(UntypedRune, unknownSize, unknownAlign),
		UntypedFloat:   newBasic(UntypedFloat, unknownSize, unknownAlign),
		UntypedComplex: newBasic(UntypedComplex, unknownSize, unknownAlign),
		UntypedString:  newBasic(UntypedString, unknownSize, unknownAlign),
		UntypedNil:     newBasic(UntypedNil, unknownSize, unknownAlign),
	}
}

// create basic types that depend on OS or architecture
func makeBasicTypes(common []*Complete, sizeOfInt uint64, alignOfFloat64 uint16) []*Complete {
	ret := append([]*Complete(nil), common...)
	ret[Int] = newBasic(Int, sizeOfInt, uint16(sizeOfInt))
	ret[Int64] = newBasic(Int64, 8, uint16(sizeOfInt))
	ret[Uint] = newBasic(Uint, sizeOfInt, uint16(sizeOfInt))
	ret[Uint64] = newBasic(Uint64, 8, uint16(sizeOfInt))
	ret[Uintptr] = newBasic(Uintptr, sizeOfInt, uint16(sizeOfInt))
	ret[Float64] = newBasic(Float64, 8, alignOfFloat64)
	ret[Complex64] = newBasic(Complex64, 8, 4)
	ret[Complex128] = newBasic(Complex128, 16, alignOfFloat64)
	ret[String] = newBasic(String, 2*sizeOfInt, uint16(sizeOfInt))
	ret[UnsafePointer] = newBasic(UnsafePointer, sizeOfInt, uint16(sizeOfInt))
	return ret
}

// create a new Basic type
func newBasic(kind Kind, size uint64, align uint16) *Complete {
	t := &Basic{
		rtype: Complete{
			size:  size,
			align: align,
			flags: flagComparable | flagComplete,
			kind:  kind,
			str:   kind.String(),
		},
	}
	if size != unknownSize {
		t.rtype.typ = t
	}
	return &t.rtype
}

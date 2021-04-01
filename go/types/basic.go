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

// Basic represents one of Go's predefined basic types
type Basic struct {
	isBasic struct{} // occupies zero bytes
	rtype   Complete
}

// *Basic implements Type

func (t *Basic) String() string {
	return t.rtype.str
}

func (t *Basic) Underlying() Type {
	return t.rtype.underlying
}

func (t *Basic) common() *Complete {
	return &t.rtype
}

// *Basic specific methods

func (t *Basic) Kind() Kind {
	return t.rtype.kind
}

func (t *Basic) Name() string {
	return t.rtype.kind.String()
}

func (t *Basic) Size() uint64 {
	return t.rtype.size
}

// create a new Basic type
func newBasic(kind Kind, size uint64) *Basic {
	t := &Basic{
		rtype: Complete{
			size:  size,
			flags: isComplete,
			kind:  kind,
			str:   kind.String(),
		},
	}
	t.rtype.underlying = t
	return t
}

var (
	basicTypes0 = makeBasicTypes0()
	basicTypes4 = makeBasicTypes(4, basicTypes0)
	basicTypes8 = makeBasicTypes(8, basicTypes0)
)

// return basic type for specified Kind and current sizeOfInt.
func BasicType(kind Kind) *Basic {
	v := basicTypes4
	if sizeOfInt > 4 {
		v = basicTypes8
	}
	return v[kind]
}

// create and return a slice containing basic types for current sizeofInt.
// use Kind as index in returned slice.
func BasicTypes() []*Basic {
	v := basicTypes4
	if sizeOfInt > 4 {
		v = basicTypes8
	}
	return append([]*Basic(nil), v...)
}

// create basic types that do not depend on sizeOfInt
func makeBasicTypes0() []*Basic {
	return []*Basic{
		Bool:       newBasic(Bool, 1),
		Int8:       newBasic(Int8, 1),
		Int16:      newBasic(Int16, 2),
		Int32:      newBasic(Int32, 4),
		Int64:      newBasic(Int64, 8),
		Uint8:      newBasic(Uint8, 1),
		Uint16:     newBasic(Uint16, 2),
		Uint32:     newBasic(Uint32, 4),
		Uint64:     newBasic(Uint64, 8),
		Float32:    newBasic(Float32, 4),
		Float64:    newBasic(Float64, 8),
		Complex64:  newBasic(Complex64, 8),
		Complex128: newBasic(Complex128, 16),
		UntypedNil: nil, // sets correct slice length
	}
}

// create basic types that depend on sizeOfInt
func makeBasicTypes(size uint64, common []*Basic) []*Basic {
	ret := append([]*Basic(nil), common...)
	ret[Int] = newBasic(Int, size)
	ret[Uint] = newBasic(Uint, size)
	ret[Uintptr] = newBasic(Uintptr, size)
	ret[String] = newBasic(String, 2*size)
	return ret
}

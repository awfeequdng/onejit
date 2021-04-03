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

import "strings"

// Basic represents one of Go's predefined basic types
type Basic struct {
	_     [0]*Basic // occupies zero bytes
	rtype Complete
}

// *Basic implements Type

func (t *Basic) String() string {
	return t.rtype.str
}

func (t *Basic) Underlying() Type {
	return t
}

func (t *Basic) common() *Complete {
	return &t.rtype
}

func (t *Basic) writeTo(b *strings.Builder, flag verbose) {
	b.WriteString(t.rtype.str)
}

// *Basic specific methods

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
	basicTypes0  = makeBasicTypes0()
	basicTypes32 = makeBasicTypes(4, basicTypes0)
	basicTypes64 = makeBasicTypes(8, basicTypes0)
)

// return basic type for specified Kind and current archSizeBits.
func BasicType(kind Kind) *Basic {
	v := basicTypes32
	if archSizeBits > 32 {
		v = basicTypes64
	}
	return v[kind]
}

// create and return a slice containing basic types for current archSizeBits.
// use Kind as index in returned slice.
func BasicTypes() []*Basic {
	v := basicTypes32
	if archSizeBits > 32 {
		v = basicTypes64
	}
	return append([]*Basic(nil), v...)
}

// create basic types that do not depend on archSizeBits
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

// create basic types that depend on archSizeBits
func makeBasicTypes(sizeOfInt uint64, common []*Basic) []*Basic {
	ret := append([]*Basic(nil), common...)
	ret[Int] = newBasic(Int, sizeOfInt)
	ret[Uint] = newBasic(Uint, sizeOfInt)
	ret[Uintptr] = newBasic(Uintptr, sizeOfInt)
	ret[String] = newBasic(String, 2*sizeOfInt)
	return ret
}

// create a new Basic type
func newBasic(kind Kind, size uint64) *Basic {
	t := &Basic{
		rtype: Complete{
			size:  size,
			flags: flagComparable | flagComplete,
			kind:  kind,
			str:   kind.String(),
		},
	}
	t.rtype.typ = t
	return t
}

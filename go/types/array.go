/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * array.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type Array struct {
	isArray struct{} // occupies zero bytes
	rtype   Complete
	extra   extra
}

// *Array implements Type

func (t *Array) String() string {
	return t.rtype.str
}

func (t *Array) Underlying() Type {
	return t
}

func (t *Array) common() *Complete {
	return &t.rtype
}

// *Array specific methods

func (t *Array) Elem() Type {
	return t.rtype.elem
}

func (t *Array) Len() uint64 {
	return uint64(t.extra.n1) | uint64(t.extra.n2)<<32
}

type arrayKey struct {
	elem Type
	len  uint64
}

var arrayMap = map[arrayKey]*Array{}

// create a new Array type
func NewArray(elem Type, len uint64) *Array {
	if len >= 1<<archSizeBits {
		panic("NewArray length exceeds archSizeBits")
	}
	key := arrayKey{elem, len}
	t := arrayMap[key]
	if t != nil {
		return t
	}
	t = &Array{
		rtype: Complete{
			size:  unknownSize,
			flags: elem.common().flags & (flagComplete | flagComparable | flagNotComparable),
			kind:  ArrayKind,
			elem:  elem,
		},
	}
	if elemsize := elem.common().size; elemsize != unknownSize {
		if elemsize != 0 && len >= (1<<archSizeBits)/elemsize {
			panic("NewArray total bytes exceed archSizeBits")
		}
		t.rtype.size = len * elemsize
		if elemsize == 0 {
			t.rtype.flags |= flagNeedPadding
		}
	}
	t.rtype.typ = t
	arrayMap[key] = t
	return t
}

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
	_     [0]*Array // occupies zero bytes
	rtype Complete
	extra extra
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
	archMaxSize := ^(^uint64(0) << archSizeBits)
	if len > archMaxSize {
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
			str:   "[" + uintToString(len) + "]" + elem.String(),
		},
	}
	if elemsize := elem.common().size; elemsize != unknownSize {
		if elemsize != 0 && len > archMaxSize/elemsize {
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

func uintToString(n uint64) string {
	if n == 0 {
		return "0"
	}
	var b [20]byte
	pos := len(b)
	for n != 0 {
		pos--
		b[pos] = '0' + byte(n%10)
		n /= 10
	}
	return string(b[pos:])
}

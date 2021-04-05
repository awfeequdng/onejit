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

import "strings"

type Array struct {
	_     [0]*Array // occupies zero bytes
	rtype Complete
	extra extra
}

// *Array implements Type

func (t *Array) String() string {
	var b strings.Builder
	t.writeTo(&b, fullPkgPath)
	return b.String()
}

func (t *Array) Underlying() Type {
	return t
}

func (t *Array) common() *Complete {
	return &t.rtype
}

func (t *Array) complete() {
	if t.rtype.flags&flagComplete != 0 {
		return
	}
	elem := t.Elem()
	len := t.Len()
	t.rtype.size = computeArraySize(elem, len)
	t.rtype.flags = computeArrayFlags(elem, len)
}

func (t *Array) writeTo(b *strings.Builder, flag verbose) {
	if flag == shortPkgName {
		b.WriteString(t.rtype.str)
		return
	}
	writeArrayTo(b, t.Len(), t.Elem(), flag)
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
			size:  computeArraySize(elem, len),
			flags: computeArrayFlags(elem, len),
			kind:  ArrayKind,
			elem:  elem,
			str:   makeArrayString(len, elem, shortPkgName),
		},
	}
	t.rtype.typ = t
	arrayMap[key] = t
	return t
}

func computeArraySize(elem Type, len uint64) uint64 {
	elemsize := elem.common().size
	if elemsize == unknownSize {
		return elemsize
	}
	archMaxSize := ^(^uint64(0) << archSizeBits)
	if elemsize != 0 && len > archMaxSize/elemsize {
		panic("NewArray total bytes exceed archSizeBits")
	}
	return len * elemsize
}

func computeArrayFlags(elem Type, len uint64) flags {
	flag := elem.common().flags & (flagComplete | flagComparable | flagNotComparable)
	if len == 0 || elem.common().size == 0 {
		flag |= flagNeedPadding
	}
	return flag
}

func makeArrayString(len uint64, elem Type, flag verbose) string {
	var b strings.Builder
	writeArrayTo(&b, len, elem, flag)
	return b.String()
}

func writeArrayTo(b *strings.Builder, len uint64, elem Type, flag verbose) {
	b.WriteByte('[')
	b.WriteString(uintToString(len))
	b.WriteByte(']')
	elem.writeTo(b, flag)
}

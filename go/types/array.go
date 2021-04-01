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
	return t.extra.n1
}

type arrayKey struct {
	elem Type
	len  uint64
}

var arrayMap = map[arrayKey]*Array{}

// create a new Array type
func NewArray(elem Type, len uint64) *Array {
	key := arrayKey{elem, len}
	t := arrayMap[key]
	if t != nil {
		return t
	}
	t = &Array{
		rtype: Complete{
			size:  unknownSize,
			flags: elem.common().flags & isComplete,
			kind:  ArrayKind,
			elem:  elem,
		},
	}
	if t.rtype.flags&isComplete != 0 {
		t.rtype.size = len * elem.common().size
	}
	t.rtype.underlying = t
	arrayMap[key] = t
	return t
}

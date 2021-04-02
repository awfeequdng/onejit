/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * slice.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type Slice struct {
	_     [0]*Slice // occupies zero bytes
	rtype Complete
}

// *Slice implements Type

func (t *Slice) String() string {
	return t.rtype.str
}

func (t *Slice) Underlying() Type {
	return t
}

func (t *Slice) common() *Complete {
	return &t.rtype
}

// *Slice specific methods

func (t *Slice) Elem() Type {
	return t.rtype.elem
}

var sliceMap = map[Type]*Slice{}

// create a new Slice type
func NewSlice(elem Type) *Slice {
	t := sliceMap[elem]
	if t != nil {
		return t
	}
	t = &Slice{
		rtype: Complete{
			size:  3 * archSizeBytes,
			flags: (elem.common().flags & flagComplete) | flagNotComparable,
			kind:  SliceKind,
			elem:  elem,
			str:   "[]" + elem.String(),
		},
	}
	t.rtype.typ = t
	sliceMap[elem] = t
	return t
}

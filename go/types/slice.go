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
	sliceTag struct{} // occupies zero bytes
	rtype    Complete
}

// *Slice implements Type

func (t *Slice) String() string {
	_ = t.sliceTag
	var b builder
	t.writeTo(&b, fullPkgPath)
	return b.String()
}

func (t *Slice) Underlying() Type {
	return t
}

func (t *Slice) common() *Complete {
	return &t.rtype
}

func (t *Slice) complete() {
	// nothing to do
}

func (t *Slice) writeTo(b *builder, flag verbose) {
	if flag == shortPkgName {
		b.WriteString(t.rtype.str)
		return
	}
	b.WriteString("[]")
	t.Elem().writeTo(b, flag)
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
	size := sizeOfPtr()
	t = &Slice{
		rtype: Complete{
			size:  3 * size,
			align: uint16(size),
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

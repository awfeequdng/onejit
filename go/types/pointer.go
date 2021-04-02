/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * pointer.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type Pointer struct {
	_     [0]*Pointer // occupies zero bytes
	rtype Complete
}

// *Pointer implements Type

func (t *Pointer) String() string {
	return t.rtype.str
}

func (t *Pointer) Underlying() Type {
	return t
}

func (t *Pointer) common() *Complete {
	return &t.rtype
}

// *Pointer specific methods

func (t *Pointer) Elem() Type {
	return t.rtype.elem
}

// create a new Pointer type
func NewPointer(elem Type) *Pointer {
	relem := elem.common()
	t := relem.ptrTo
	if t != nil {
		return t
	}
	t = &Pointer{
		rtype: Complete{
			size:  archSizeBytes,
			flags: (relem.flags & flagComplete) | flagComparable,
			kind:  PtrKind,
			elem:  elem,
			str:   "*" + elem.String(),
		},
	}
	t.rtype.typ = t
	relem.ptrTo = t
	return t
}

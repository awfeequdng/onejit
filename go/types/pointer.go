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

import "github.com/cosmos72/onejit/go/io"

type Pointer struct {
	pointerTag struct{} // occupies zero bytes
	rtype      Complete
}

// *Pointer implements Type

func (t *Pointer) String() string {
	_ = t.pointerTag
	var b builder
	t.WriteTo(&b, fullPkgPath)
	return b.String()
}

func (t *Pointer) Underlying() Type {
	return t
}

func (t *Pointer) common() *Complete {
	return &t.rtype
}

func (t *Pointer) complete() {
	// nothing to do
}

func (t *Pointer) WriteTo(dst io.StringWriter, flag verbose) {
	if flag == shortPkgName {
		dst.WriteString(t.rtype.str)
		return
	}
	dst.WriteString("*")
	t.Elem().WriteTo(dst, flag)
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
	size := sizeOfPtr()
	t = &Pointer{
		rtype: Complete{
			size:  size,
			align: uint16(size),
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

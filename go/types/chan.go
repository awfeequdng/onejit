/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * chan.go
 *
 *  Created on: Mar 31, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type Chan struct {
	isChan struct{} // occupies zero bytes
	rtype  Complete
}

// *Chan implements Type

func (t *Chan) String() string {
	return t.rtype.str
}

func (t *Chan) Underlying() Type {
	return t
}

func (t *Chan) common() *Complete {
	return &t.rtype
}

// *Chan specific methods

func (t *Chan) Elem() Type {
	return t.rtype.elem
}

func (t *Chan) Dir() ChanDir {
	return ChanDir(t.rtype.flags) & BothDir
}

type chanKey struct {
	dir  ChanDir
	elem Type
}

var chanMap = map[chanKey]*Chan{}

// create a new Chan type
func NewChan(dir ChanDir, elem Type) *Chan {
	dir &= BothDir
	key := chanKey{dir, elem}
	t := chanMap[key]
	if t != nil {
		return t
	}
	t = &Chan{
		rtype: Complete{
			size:  archSizeBytes,
			flags: flags(dir) | (elem.common().flags & flagComplete) | flagNotComparable,
			kind:  ChanKind,
			elem:  elem,
		},
	}
	t.rtype.typ = t
	chanMap[key] = t
	return t
}

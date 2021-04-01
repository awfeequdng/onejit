/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * map.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type Map struct {
	isMap struct{} // occupies zero bytes
	rtype Complete
	extra extra
}

// *Map implements Type

func (t *Map) String() string {
	return t.rtype.str
}

func (t *Map) Underlying() Type {
	return t
}

func (t *Map) common() *Complete {
	return &t.rtype
}

// *Map specific methods

func (t *Map) Key() Type {
	return t.extra.types[0]
}

func (t *Map) Elem() Type {
	return t.rtype.elem
}

type mapKey struct {
	keyElem [2]Type
}

var mapMap = map[mapKey]*Map{}

// create a new Map type
func NewMap(key Type, elem Type) *Map {
	k := mapKey{[2]Type{key, elem}}
	t := mapMap[k]
	if t != nil {
		return t
	}
	t = &Map{
		rtype: Complete{
			size:  sizeOfInt,
			flags: key.common().flags & elem.common().flags & isComplete,
			kind:  MapKind,
			elem:  elem,
			str:   "map[" + key.String() + "]" + elem.String(),
		},
		extra: extra{
			types: k.keyElem[0:1],
		},
	}
	t.rtype.underlying = t
	t.rtype.extra = &t.extra
	mapMap[k] = t
	return t
}

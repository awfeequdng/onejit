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

import "strings"

type Map struct {
	_     [0]*Map // occupies zero bytes
	rtype Complete
	extra extra
}

// *Map implements Type

func (t *Map) String() string {
	var b strings.Builder
	t.writeTo(&b, fullPkgPath)
	return b.String()
}

func (t *Map) Underlying() Type {
	return t
}

func (t *Map) common() *Complete {
	return &t.rtype
}

func (t *Map) writeTo(b *strings.Builder, flag verbose) {
	if flag == shortPkgName {
		b.WriteString(t.rtype.str)
		return
	}
	writeMapTo(b, t.Key(), t.Elem(), flag)
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
	if key.common().flags&flagNotComparable != 0 {
		panic("NewMap: map key type is not comparable")
	}
	k := mapKey{[2]Type{key, elem}}
	t := mapMap[k]
	if t != nil {
		return t
	}
	t = &Map{
		rtype: Complete{
			size:  archSizeBytes,
			flags: (key.common().flags & elem.common().flags & flagComplete) | flagNotComparable,
			kind:  MapKind,
			elem:  elem,
			str:   makeMapString(key, elem, shortPkgName),
		},
		extra: extra{
			types: k.keyElem[0:1],
		},
	}
	t.rtype.typ = t
	t.rtype.extra = &t.extra
	mapMap[k] = t
	return t
}

func makeMapString(key Type, elem Type, flag verbose) string {
	var b strings.Builder
	writeMapTo(&b, key, elem, flag)
	return b.String()
}

func writeMapTo(b *strings.Builder, key Type, elem Type, flag verbose) {
	b.WriteString("map[")
	key.writeTo(b, flag)
	b.WriteByte(']')
	elem.writeTo(b, flag)
}

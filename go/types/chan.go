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

import "github.com/cosmos72/onejit/go/io"

type Chan struct {
	chanTag struct{} // occupies zero bytes
	rtype   Complete
}

// *Chan implements Type

func (t *Chan) String() string {
	_ = t.chanTag
	var b builder
	t.WriteTo(&b, fullPkgPath)
	return b.String()
}

func (t *Chan) Underlying() Type {
	return t
}

func (t *Chan) common() *Complete {
	return &t.rtype
}

func (t *Chan) complete() {
	if t.rtype.hash == unknownHash {
		t.rtype.hash = computeChanHash(t.Dir(), t.Elem())
	}
}

func (t *Chan) WriteTo(dst io.StringWriter, flag verbose) {
	if flag == shortPkgName {
		dst.WriteString(t.rtype.str)
		return
	}
	writeChanTo(dst, t.Dir(), t.Elem(), flag)
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
	size := sizeOfPtr()
	t = &Chan{
		rtype: Complete{
			size:  size,
			align: uint16(size),
			flags: flags(dir) | (elem.common().flags & flagComplete) | flagNotComparable,
			kind:  ChanKind,
			elem:  elem,
			hash:  computeChanHash(dir, elem),
			str:   makeChanString(dir, elem, shortPkgName),
		},
	}
	t.rtype.typ = t
	chanMap[key] = t
	return t
}

func computeChanHash(dir ChanDir, elem Type) hash {
	elemhash := elem.common().hash
	if elemhash == unknownHash {
		return unknownHash
	}
	return elemhash.String("chan").Uint16(uint16(dir))
}

func makeChanString(dir ChanDir, elem Type, flag verbose) string {
	var b builder
	writeChanTo(&b, dir, elem, flag)
	return b.String()
}

func writeChanTo(dst io.StringWriter, dir ChanDir, elem Type, flag verbose) {
	if dir == RecvDir {
		dst.WriteString("<-")
	}
	dst.WriteString("chan")
	if dir == SendDir {
		dst.WriteString("<-")
	}
	dst.WriteString(" ")
	elem.WriteTo(dst, flag)
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * named.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/strings"
)

// represents a named type
type Named struct {
	namedTag struct{} // occupies zero bytes
	rtype    Complete
	extra    extra
}

// *Named implements Type

func (t *Named) String() string {
	_ = t.namedTag
	var b builder
	t.WriteTo(&b, fullPkgPath)
	return b.String()
}

// returns nil if SetUnderlying() was not invoked yet
func (t *Named) Underlying() Type {
	return t.extra.underlying
}

func (t *Named) common() *Complete {
	return &t.rtype
}

func (t *Named) WriteTo(dst io.StringWriter, flag verbose) {
	if flag == shortPkgName {
		dst.WriteString(t.rtype.str)
		return
	}
	strings.WriteQualifiedName(dst, t.Name(), t.PkgPath(), flag)
}

// *Named specific methods

func (t *Named) Name() string {
	return t.extra.name
}

func (t *Named) PkgPath() string {
	return t.extra.pkgPath
}

// Set underlying type of t to be the same as underlying type of u.
// u can also be a *Named (and possibly incomplete) type:
// its underlying type will be retrieved by CompleteTypes(t)
func (t *Named) SetUnderlying(u Type) {
	if t.extra.underlying != nil {
		panic("SetUnderlying invoked already")
	} else if u == nil {
		panic("SetUnderlying underlying type is nil")
	}
	u = traverseUnderlyingCheckLoop(t, u)
	fillFromUnderlying(&t.rtype, u.common())

	t.extra.underlying = u
}

// return number of declared methods.
// Ignores methods from embedded fields
func (t *Named) NumMethod() int {
	return len(t.extra.methods)
}

// return i-th declared method.
// Ignores methods from embedded fields
func (t *Named) Method(i int) Method {
	return t.extra.methods[i]
}

// add a method. always appends to the list of methods,
// even if another method with the same name already exists.
func (t *Named) AddMethod(mtd *Method) {
	_ = mtd.Type.(*Func)
	ms := t.extra.methods
	count := len(ms)
	ms = append(ms, *mtd)   // append method as-is
	ms[count].Index = count // then fix its copy
	t.extra.methods = ms    // update, in case append() above reallocates
}

// create a new Named type
func NewNamed(name string, pkgPath string) *Named {
	str := name
	if len(pkgPath) != 0 {
		fullStr := pkgPath + "." + name
		fullLen := len(fullStr)
		pkgLen := len(pkgPath)
		name = fullStr[fullLen-len(name):]
		pkgPath = fullStr[:pkgLen]
		str = fullStr[pkgLen-len(strings.Basename(pkgPath)):]
	}
	t := &Named{
		rtype: Complete{
			size:  unknownSize,
			align: unknownAlign,
			kind:  Invalid, // not known yet
			hash:  computeNamedHash(name, pkgPath),
			str:   str,
		},
		extra: extra{
			name:    name,
			pkgPath: pkgPath,
		},
	}
	t.rtype.typ = t
	t.rtype.extra = &t.extra
	return t
}

func traverseUnderlyingCheckLoop(t Type, u Type) Type {
	next := u
	for {
		if t == next {
			panic("SetUnderlying loops are not allowed")
		}
		u = next
		next = u.Underlying()
		if next == u || next == nil {
			// for unnamed types, Type.Underlying() returns the receiver
			break
		}
	}
	return u
}

func fillFromUnderlying(r *Complete, u *Complete) {
	r.size = u.size
	r.align = u.align
	r.flags = u.flags
	r.kind = u.kind
	r.elem = u.elem
	if ux := u.extra; ux != nil {
		rx := r.extra
		rx.n1 = ux.n1
		rx.n2 = ux.n2
		rx.types = ux.types
		rx.fields = ux.fields
	}
}

func (t *Named) complete() {
	if t.rtype.flags&flagComplete != 0 {
		return
	}
	u := t.extra.underlying
	for u != nil {
		if t == u {
			panic("CompleteTypes: named underlying loops are not allowed")
		}
		if named, _ := u.(*Named); named != nil {
			u = named.extra.underlying
		} else {
			break
		}
	}
	if u == nil {
		panic("CompleteTypes: named underlying type is nil")
	}
	if u != t.extra.underlying {
		fillFromUnderlying(&t.rtype, u.common())
		t.extra.underlying = u
	}
}

// hash of a named type cannot depend on its underlying type,
// because named types can be recursive i.e. refer to themselves
func computeNamedHash(name string, pkgPath string) hash {
	return hashInit().String(name).String(pkgPath)
}

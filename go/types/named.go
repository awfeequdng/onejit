/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * basic.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

// TODO: duplicate Method, one struct has 'Type Type' and the other has 'Type *Complete'
type Method struct {
	Address uint64
	Type    Type
	QualifiedName
	Index int
}

type Named struct {
	isNamed struct{} // occupies zero bytes
	rtype   Complete
	extra   extra
}

// *Named implements Type

func (t *Named) String() string {
	return t.rtype.str
}

// returns nil if SetUnderlying() was not invoked yet
func (t *Named) Underlying() Type {
	return t.rtype.underlying
}

func (t *Named) common() *Complete {
	return &t.rtype
}

// *Named specific methods

func (t *Named) Name() string {
	return t.extra.qname.Name
}

func (t *Named) PkgPath() string {
	return t.extra.qname.PkgPath
}

func (t *Named) SetUnderlying(u Type) {
	if t.rtype.underlying != nil {
		panic("SetUnderlying invoked already")
	} else if u == nil {
		return
	}
	u = u.Underlying()
	if u == nil {
		return
	}
	rt, ru := &t.rtype, u.common()
	rt.size = ru.size
	rt.flags = ru.flags
	rt.kind = ru.kind
	rt.underlying = u
	if rux := ru.extra; rux != nil {
		rtx := &t.extra
		rtx.n1 = rux.n1
		rtx.n2 = rux.n2
		rtx.fields = rux.fields
	}
}

// return number of declared methods.
// Ignores methods from embedded fields
func (t *Named) NumMethod() int {
	return len(t.extra.fields)
}

// return i-th declared method.
// Ignores methods from embedded fields
func (t *Named) Method(i int) Func {
	field := &t.extra.fields[i]
	return Func{
		Address:       field.Offset, // hack
		Type:          field.Type.(*Signature),
		QualifiedName: field.QualifiedName,
	}
}

// add a method. always appends to the list of methods,
// even if another method with the same name already exists.
func (t *Named) AddMethod(m *Func) {
	fields := &t.extra.fields
	*fields = append(*fields, Field{
		Type:          m.Type,
		QualifiedName: m.QualifiedName,
		Offset:        m.Address, // hack
	})
}

// create a new Named type
func NewNamed(name string, pkgPath string) *Named {
	qname := name
	if n := len(pkgPath); n != 0 {
		qname = pkgPath + "." + name
		pkgPath = qname[:n]
		name = qname[n+1:]
	}
	t := &Named{
		rtype: Complete{
			size: unknownSize,
			kind: Invalid, // not known yet
			str:  qname,
		},
		extra: extra{
			qname: QualifiedName{name, pkgPath},
		},
	}
	t.rtype.extra = &t.extra
	return t
}

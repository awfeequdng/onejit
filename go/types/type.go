/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * type.go
 *
 *  Created on: Mar 31, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type Type *rtype

type rtype struct {
	methodAddress []uintptr
	size          uintptr
	flags         flags // channel direction, variadic function
	kind          Kind
	underlying    *rtype
	methods       *[]Method      // nil if no methods
	extra         *extra         // map's key type, function's param and result types, struct fields
	elem          *rtype         // elem type of array, chan, map, ptr, slice
	ptrTo         *rtype         // type of pointer to this type
	qname         *QualifiedName // nil for unnamed types
	str           string         // compute lazily?
}

type extra struct {
	numIn, numOut int // # of function parameters and results
	params        []*rtype
	fields        []Field // struct fields
}

type flags uint32
type ChanDir flags

const (
	RecvDir ChanDir = 1
	SendDir ChanDir = 2
	BothDir         = RecvDir | SendDir

	isVariadic flags = 4
)

func (t *rtype) Kind() Kind {
	return t.kind
}

func (t *rtype) Name() string {
	if t.qname != nil {
		return t.qname.Name
	}
	return ""
}

func (t *rtype) PkgPath() string {
	if t.qname != nil {
		return t.qname.PkgPath
	}
	return ""
}

func (t *rtype) Size() uintptr {
	return t.size
}

func (t *rtype) String() string {
	return t.str
}

func (t *rtype) Underlying() Type {
	return t.underlying
}

// commonly supported methods

func (t *rtype) Elem() Type {
	switch k := t.kind; k {
	case Array, Chan, Map, Ptr, Slice:
		return t.elem
	default:
		panic("Elem of invalid type")
	}
}

func (t *rtype) Key() Type {
	if t.kind == Map {
		return t.extra.params[0]
	}
	panic("Key of invalid type")
}

// Method-related methods

func (t *rtype) NumMethod() int {
	return len(t.methodAddress)
}

func (t *rtype) Method(i int) Method {
	return (*t.methods)[i]
}

// Struct-related methods

func (t *rtype) NumField() int {
	if t.kind == Struct {
		return len(t.extra.fields)
	}
	panic("NumField of invalid type")
}

func (t *rtype) Field(i int) Field {
	if t.kind == Struct {
		return t.extra.fields[i]
	}
	panic("Field of invalid type")
}

// Func-related methods

func (t *rtype) NumIn() int {
	if t.kind == Func {
		return t.extra.numIn
	}
	panic("NumIn of invalid type")
}

func (t *rtype) NumOut() int {
	if t.kind == Func {
		return t.extra.numOut
	}
	panic("NumOut of invalid type")
}

func (t *rtype) In(i int) Type {
	if t.kind == Func {
		extra := t.extra
		return extra.params[0:extra.numIn][i]
	}
	panic("In of invalid type")
}

func (t *rtype) Out(i int) Type {
	if t.kind == Func {
		extra := t.extra
		return extra.params[extra.numIn:][i]
	}
	panic("Out of invalid type")
}

func (t *rtype) IsVariadic() bool {
	if t.kind == Func {
		return t.flags&isVariadic != 0
	}
	panic("IsVariadic of invalid type")
}

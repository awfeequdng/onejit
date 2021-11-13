/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * complete.go
 *
 *  Created on: Mar 31, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

// Complete represent a Go type. It is similar in spirit to reflect.Type
type Complete struct {
	size  uint64   // in bytes. uint64 to support compiling 64-bit code from 32-bit systems
	align uint16   // in bytes. also depends on target OS/architecture
	flags flags    // channel direction, variadic function
	kind  Kind     // kind
	typ   Type     // type, needed to convert Complete to Type
	elem  Type     // elem type of array, chan, map, ptr, slice
	hash  hash     // type's hash
	extra *extra   // array length, function parameters/results, map key Type, struct fields, named name and underlying
	ptrTo *Pointer // type of pointer to this type
	str   string   // compute lazily?
}

type extra struct {
	n1, n2     uint32   // # of function parameters and results, or array length
	types      []Type   // function parameters and results, interface embedded types, or map's key type
	fields     []Field  // struct fields
	methods    []Method // interface methods, named methods, or methods from embedded struct fields
	name       string
	pkgPath    string
	underlying Type
}

// *Complete should NOT implement Type

func (t *Complete) String() string {
	if t == nil {
		return ""
	}
	return t.str
}

// convert *Complete to Type
func (t *Complete) Type() Type {
	if t == nil {
		return nil
	}
	return t.typ
}

func (t *Complete) Underlying() *Complete {
	if t.extra != nil {
		if u := t.extra.underlying; u != nil {
			return u.common()
		}
	}
	return t
}

// methods valid for any Kind

func (t *Complete) Align() uint64 {
	return uint64(t.align)
}

func (t *Complete) Comparable() bool {
	return t.flags&flagComparable != 0
}

func (t *Complete) Kind() Kind {
	if t == nil {
		return Invalid
	}
	return t.kind
}

func (t *Complete) Name() string {
	if t.extra != nil {
		return t.extra.name
	} else if t.kind.IsBasic() {
		return t.str
	}
	return ""
}

func (t *Complete) PkgPath() string {
	if t.extra != nil {
		return t.extra.pkgPath
	}
	return ""
}

func (t *Complete) Size() uint64 {
	return t.size
}

// commonly supported methods

func (t *Complete) Elem() *Complete {
	switch k := t.kind; k {
	case ArrayKind, ChanKind, MapKind, PtrKind, SliceKind:
		return t.elem.common()
	default:
		panic("Elem of invalid type")
	}
}

func (t *Complete) Len() uint64 {
	if t.kind == ArrayKind {
		return uint64(t.extra.n1) | uint64(t.extra.n2)<<32
	}
	panic("Len of invalid type")
}

// Chan-related methods

func (t *Complete) ChanDir() ChanDir {
	if t.kind == ChanKind {
		return ChanDir(t.flags) & BothDir
	}
	panic("ChanDir of invalid type")
}

// Func-related methods

func (t *Complete) IsVariadic() bool {
	if t.kind == FuncKind {
		return t.flags&flagVariadic != 0
	}
	panic("IsVariadic of invalid type")
}

func (t *Complete) NumIn() int {
	if t.kind == FuncKind {
		return int(t.extra.n1)
	}
	panic("NumIn of invalid type")
}

func (t *Complete) NumOut() int {
	if t.kind == FuncKind {
		return int(t.extra.n2)
	}
	panic("NumOut of invalid type")
}

func (t *Complete) In(i int) *Complete {
	if t.kind == FuncKind {
		extra := t.extra
		return extra.types[0:extra.n1][i].common()
	}
	panic("In of invalid type")
}

func (t *Complete) Out(i int) *Complete {
	if t.kind == FuncKind {
		extra := t.extra
		return extra.types[extra.n1 : extra.n1+extra.n2][i].common()
	}
	panic("Out of invalid type")
}

// Map-related methods

func (t *Complete) Key() *Complete {
	if t.kind == MapKind {
		return t.extra.types[0].common()
	}
	panic("Key of invalid type")
}

// Named-related methods

func (t *Complete) NumMethod() int {
	if t.extra == nil {
		return 0
	}
	return len(t.extra.methods)
}

func (t *Complete) Method(i int) CompleteMethod {
	m := &t.extra.methods[i]
	return CompleteMethod{
		Type:    m.Type.common(),
		Name:    m.Name,
		PkgPath: m.PkgPath,
		Index:   m.Index,
	}
}

// Struct-related methods

func (t *Complete) NumField() int {
	if t.kind == StructKind {
		return len(t.extra.fields)
	}
	panic("NumField of invalid type")
}

func (t *Complete) Field(i int) CompleteField {
	if t.kind != StructKind {
		panic("Field of invalid type")
	}
	f := &t.extra.fields[i]
	return CompleteField{
		Type:     f.Type.common(),
		Name:     f.Name,
		PkgPath:  f.PkgPath,
		Tag:      f.Tag,
		Offset:   f.Offset,
		Index:    f.Index,
		Embedded: f.Embedded,
	}
}

func (t *Complete) field(i int) *Field {
	return &t.extra.fields[i]
}

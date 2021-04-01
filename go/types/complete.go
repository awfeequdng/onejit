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

// similar in spirit to Go reflect.Type
type Complete struct {
	methods    *[]Method // nil if no methods
	size       uint64    // in bytes. we also support compiling 64-bit code from 32-bit systems
	flags      flags     // channel direction, variadic function
	kind       Kind
	underlying Type
	extra      *extra   // array len, map's key type, function's param and result types, struct fields
	elem       Type     // elem type of array, chan, map, ptr, slice
	ptrTo      *Pointer // type of pointer to this type
	str        string   // compute lazily?
}

type extra struct {
	n1, n2 uint64        // # of function parameters and results, or array length
	types  []Type        // map's key type, or function parameters
	fields []Field       // struct fields
	qname  QualifiedName // used only by *Named
}

// *Complete should NOT implement Type

func (t *Complete) String() string {
	return t.str
}

func (t *Complete) Underlying() *Complete {
	return t.underlying.common()
}

// methods valid for any Kind

func (t *Complete) Kind() Kind {
	return t.kind
}

func (t *Complete) Name() string {
	if t.extra != nil {
		return t.extra.qname.Name
	}
	return ""
}

func (t *Complete) PkgPath() string {
	if t.extra != nil {
		return t.extra.qname.PkgPath
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
		return t.extra.n1
	}
	panic("Len of invalid type")
}

// Func-related methods

func (t *Complete) IsVariadic() bool {
	if t.kind == FuncKind {
		return t.flags&isVariadic != 0
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
		return extra.types[0:extra.n1][i].common()
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
	if t.methods == nil {
		return 0
	}
	return len(*t.methods)
}

func (t *Complete) Method(i int) Method {
	return (*t.methods)[i]
}

// Struct-related methods

func (t *Complete) NumField() int {
	if t.kind == StructKind {
		return len(t.extra.fields)
	}
	panic("NumField of invalid type")
}

func (t *Complete) Field(i int) Field {
	if t.kind == StructKind {
		return t.extra.fields[i]
	}
	panic("Field of invalid type")
}

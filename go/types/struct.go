/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * struct.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import "strings"

type CompleteField struct {
	Type     *Complete
	Name     string
	PkgPath  string
	Tag      string
	Offset   uint64
	Index    int
	Embedded bool
}

type Field struct {
	Type     Type
	Name     string
	PkgPath  string
	Tag      string
	Offset   uint64
	Index    int
	Embedded bool
}

type Struct struct {
	isStruct struct{} // occupies zero bytes
	rtype    Complete
	extra    extra
}

// *Struct implements Type

func (t *Struct) String() string {
	return t.rtype.str
}

func (t *Struct) Underlying() Type {
	return t
}

func (t *Struct) common() *Complete {
	return &t.rtype
}

// *Struct specific methods

func (t *Struct) NumField() int {
	return len(t.extra.fields)
}

func (t *Struct) Field(i int) Field {
	return t.extra.fields[i]
}

var structMap = map[interface{}]*Struct{}

// create a new Struct type
func NewStruct(fields []Field) *Struct {
	key, fields := makeStructKey(fields)
	t := structMap[key]
	if t != nil {
		return t
	}
	t = &Struct{
		rtype: Complete{
			size:  computeStructSize(fields),
			flags: computeStructFlags(fields),
			kind:  StructKind,
			str:   makeStructString(fields),
		},
		extra: extra{
			fields: fields,
		},
	}
	t.rtype.typ = t
	t.rtype.extra = &t.extra
	structMap[key] = t
	return t
}

type (
	structKey0   = [0]Field
	structKey2   = [2]Field
	structKey4   = [4]Field
	structKey16  = [16]Field
	structKey64  = [64]Field
	structKey256 = [256]Field
	structKey1k  = [1024]Field
	structKey4k  = [4096]Field
	structKey16k = [16384]Field
	structKey64k = [65536]Field
)

func makeStructKey(fields []Field) (interface{}, []Field) {
	n := len(fields)
	const maxn = 65536
	if n > maxn {
		panic("NewStruct: too many fields")
	}
	var ret interface{}
	var fieldscopy []Field
	if n == 0 {
		ret = structKey0{}
	} else if n <= 2 {
		key := structKey2{}
		fieldscopy = fillStructKey(key[:], fields)
		ret = key // copies key, must be done after fillStructKey
	} else if n <= 4 {
		key := structKey4{}
		fieldscopy = fillStructKey(key[:], fields)
		ret = key
	} else if n <= 16 {
		key := structKey16{}
		fieldscopy = fillStructKey(key[:], fields)
		ret = key
	} else if n <= 256 {
		key := structKey256{}
		fieldscopy = fillStructKey(key[:], fields)
		ret = key
	} else if n <= 1024 {
		key := structKey1k{}
		fieldscopy = fillStructKey(key[:], fields)
		ret = key
	} else if n <= 4096 {
		key := structKey4k{}
		fieldscopy = fillStructKey(key[:], fields)
		ret = key
	} else if n <= 16384 {
		key := structKey16k{}
		fieldscopy = fillStructKey(key[:], fields)
		ret = key
	} else {
		key := structKey64k{}
		fieldscopy = fillStructKey(key[:], fields)
		ret = key
	}
	return ret, fieldscopy
}

func fillStructKey(fieldscopy, fields []Field) []Field {
	fieldscopy = fieldscopy[:len(fields)]
	copy(fieldscopy, fields)
	for i := range fieldscopy {
		field := &fieldscopy[i]
		field.Offset = unknownSize
		field.Index = i
	}
	return fieldscopy
}

func computeStructSize(fields []Field) uint64 {
	size := uint64(0)
	for i := range fields {
		fieldsize := fields[i].Type.common().size
		if fieldsize == unknownSize {
			return fieldsize
		}
		size += fieldsize
	}
	return size
}

func computeStructFlags(fields []Field) flags {
	flag := flagComplete
	for i := range fields {
		flag &= fields[i].Type.common().flags
	}
	if n := len(fields); n == 0 || fields[n-1].Type.common().flags&flagNeedPadding != 0 {
		flag |= flagNeedPadding
	}
	return flag
}

func makeStructString(fields []Field) string {
	var b strings.Builder
	b.WriteString("struct {")
	for i, field := range fields {
		if i == 0 {
			b.WriteByte(' ')
		} else {
			b.WriteString("; ")
		}
		if !field.Embedded {
			b.WriteString(field.Name)
			b.WriteByte(' ')
		}
		b.WriteString(field.Type.String())
	}
	if len(fields) == 0 {
		b.WriteByte('}')
	} else {
		b.WriteString(" }")
	}
	return b.String()
}

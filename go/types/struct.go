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

type Struct struct {
	_     [0]*Struct // occupies zero bytes
	rtype Complete
	extra extra
}

// *Struct implements Type

func (t *Struct) String() string {
	var b strings.Builder
	t.writeTo(&b, fullPkgPath)
	return b.String()
}

func (t *Struct) Underlying() Type {
	return t
}

func (t *Struct) common() *Complete {
	return &t.rtype
}

func (t *Struct) complete() {
	if t.rtype.flags&flagComplete != 0 {
		return
	}
	// TODO
}

func (t *Struct) writeTo(b *strings.Builder, flag verbose) {
	if flag == shortPkgName {
		b.WriteString(t.rtype.str)
		return
	}
	writeStructTo(b, t.extra.fields, flag)
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
			str:   makeStructString(fields, shortPkgName),
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
	flagsAnd := ^flags(0)
	flagsOr := flags(0)
	for i := range fields {
		flag_i := fields[i].Type.common().flags
		flagsAnd &= flag_i
		flagsOr |= flag_i
	}
	// a struct is complete if all its fields are complete
	flag := flagsAnd & flagComplete
	if flagsOr&flagNotComparable != 0 {
		// at least one field is not comparable => the whole struct is not comparable
		flag |= flagNotComparable
	} else if flagsAnd&flagComparable != 0 {
		// all fields are comparable => the whole struct is comparable
		flag |= flagComparable
	} else {
		// not known yet whether struct is comparable or not
	}
	if n := len(fields); n == 0 || fields[n-1].Type.common().flags&flagNeedPadding != 0 {
		flag |= flagNeedPadding
	}
	return flag
}

func makeStructString(fields []Field, flag verbose) string {
	var b strings.Builder
	writeStructTo(&b, fields, flag)
	return b.String()
}

func writeStructTo(b *strings.Builder, fields []Field, flag verbose) {
	b.WriteString("struct {")
	for i := range fields {
		if i == 0 {
			b.WriteByte(' ')
		} else {
			b.WriteString("; ")
		}
		fields[i].writeTo(b, flag)
	}
	if len(fields) != 0 {
		b.WriteByte(' ')
	}
	b.WriteByte('}')
}

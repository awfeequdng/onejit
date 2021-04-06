/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * func.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"strings"
)

// Func represents the type of a function
type Func struct {
	_     [0]*Func // occupies zero bytes
	rtype Complete
	extra extra
}

// *Func implements Type

func (t *Func) String() string {
	var b strings.Builder
	t.writeTo(&b, fullPkgPath)
	return b.String()
}

func (t *Func) Underlying() Type {
	return t
}

func (t *Func) common() *Complete {
	return &t.rtype
}

func (t *Func) complete() {
	// nothing to do
}

func (t *Func) writeTo(b *strings.Builder, flag verbose) {
	if flag == shortPkgName {
		b.WriteString(t.rtype.str)
		return
	}
	b.WriteString("func")
	writeFuncTo(b, t.in(), t.out(), t.IsVariadic(), flag)
}

// *Func specific methods

func (t *Func) IsVariadic() bool {
	return t.rtype.flags&flagVariadic != 0
}

func (t *Func) NumIn() int {
	return int(t.extra.n1)
}

func (t *Func) NumOut() int {
	return int(t.extra.n2)
}

func (t *Func) In(i int) Type {
	return t.in()[i]
}

func (t *Func) Out(i int) Type {
	return t.out()[i]
}

func (t *Func) in() []Type {
	extra := t.extra
	return extra.types[0:extra.n1]
}

func (t *Func) out() []Type {
	extra := t.extra
	return extra.types[extra.n1:]
}

var funcMap = map[interface{}]*Func{}

// create a new Func type
func NewFunc(in []Type, out []Type, variadic bool) *Func {
	key, types := makeFuncKey(in, out, variadic)
	t := funcMap[key]
	if t != nil {
		return t
	}
	flag := flagsAnd(in) & flagsAnd(out) & flagComplete
	if variadic {
		flag |= flagVariadic
	}
	t = &Func{
		rtype: Complete{
			size:  sizeOfPtr(),
			flags: flag | flagNotComparable,
			kind:  FuncKind,
			str:   makeFuncString(in, out, variadic, shortPkgName),
		},
		extra: extra{
			n1:    uint32(len(in)),
			n2:    uint32(len(out)),
			types: types,
		},
	}
	t.rtype.typ = t
	t.rtype.extra = &t.extra
	funcMap[key] = t
	return t
}

type (
	funcKey0 = struct{}
	funcKey2 = struct {
		n1, n2 uint16
		inout  [2]Type
	}
	funcKey4 = struct {
		n1, n2 uint16
		inout  [4]Type
	}
	funcKey16 = struct {
		n1, n2 uint16
		inout  [16]Type
	}
	funcKey64 = struct {
		n1, n2 uint16
		inout  [64]Type
	}
	funcKey256 = struct {
		n1, n2 uint16
		inout  [256]Type
	}
	funcKey1k = struct {
		n1, n2 uint16
		inout  [1024]Type
	}
	funcKey4k = struct {
		n1, n2 uint16
		inout  [4096]Type
	}
	funcKey16k = struct {
		n1, n2 uint16
		inout  [16384]Type
	}
	funcKey64k = struct {
		n1, n2 uint16
		inout  [65536]Type
	}
)

func makeFuncKey(in []Type, out []Type, variadic bool) (ret interface{}, types []Type) {
	n1, n2 := uint(len(in)), uint(len(out))
	n := n1 + n2
	const maxn = 65536
	if n1 >= maxn || n2 >= maxn/2 || n > maxn {
		panic("NewFunc: too many function parameters and results")
	}
	if variadic {
		n2 |= maxn / 2
	}
	if n == 0 {
		ret = funcKey0{}
	} else if n <= 2 {
		key := funcKey2{n1: uint16(n1), n2: uint16(n2)}
		types = fillFuncKey(key.inout[:], in, out)
		ret = key // copies key, must be done after fillSigKey
	} else if n <= 4 {
		key := funcKey4{n1: uint16(n1), n2: uint16(n2)}
		types = fillFuncKey(key.inout[:], in, out)
		ret = key
	} else if n <= 16 {
		key := funcKey16{n1: uint16(n1), n2: uint16(n2)}
		types = fillFuncKey(key.inout[:], in, out)
		ret = key
	} else if n <= 256 {
		key := funcKey256{n1: uint16(n1), n2: uint16(n2)}
		types = fillFuncKey(key.inout[:], in, out)
		ret = key
	} else if n <= 1024 {
		key := funcKey1k{n1: uint16(n1), n2: uint16(n2)}
		types = fillFuncKey(key.inout[:], in, out)
		ret = key
	} else if n <= 4096 {
		key := funcKey4k{n1: uint16(n1), n2: uint16(n2)}
		types = fillFuncKey(key.inout[:], in, out)
		ret = key
	} else if n <= 16384 {
		key := funcKey16k{n1: uint16(n1), n2: uint16(n2)}
		types = fillFuncKey(key.inout[:], in, out)
		ret = key
	} else {
		key := funcKey64k{n1: uint16(n1), n2: uint16(n2)}
		types = fillFuncKey(key.inout[:], in, out)
		ret = key
	}
	return ret, types
}

func fillFuncKey(dst, in, out []Type) []Type {
	n1, n2 := len(in), len(out)
	dst = dst[:n1+n2]
	copy(dst[:n1], in)
	copy(dst[n1:], out)
	return dst
}

func makeFuncString(in []Type, out []Type, variadic bool, flag verbose) string {
	var b strings.Builder
	b.WriteString("func")
	writeFuncTo(&b, in, out, variadic, flag)
	return b.String()
}

// does NOT write "func" prefix
func writeFuncTo(b *strings.Builder, in []Type, out []Type, variadic bool, flag verbose) {
	b.WriteByte('(')
	for i, t := range in {
		if i != 0 {
			b.WriteString(", ")
		}
		if i+1 == len(in) && variadic {
			b.WriteString("...")
		}
		t.writeTo(b, flag)
	}
	b.WriteByte(')')

	switch len(out) {
	case 0:
	case 1:
		b.WriteByte(' ')
		out[0].writeTo(b, flag)
	default:
		b.WriteString(" (")
		for i, t := range out {
			if i != 0 {
				b.WriteString(", ")
			}
			t.writeTo(b, flag)
		}
		b.WriteByte(')')
	}
}

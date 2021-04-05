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

import "strings"

// Signature represents the type of a function
type Signature struct {
	_     [0]*Signature // occupies zero bytes
	rtype Complete
	extra extra
}

// *Signature implements Type

func (t *Signature) String() string {
	var b strings.Builder
	t.writeTo(&b, fullPkgPath)
	return b.String()
}

func (t *Signature) Underlying() Type {
	return t
}

func (t *Signature) common() *Complete {
	return &t.rtype
}

func (t *Signature) complete() {
	if t.rtype.flags&flagComplete != 0 {
		return
	}
	// TODO
}

func (t *Signature) writeTo(b *strings.Builder, flag verbose) {
	if flag == shortPkgName {
		b.WriteString(t.rtype.str)
		return
	}
	b.WriteString("func")
	writeSignatureTo(b, t.in(), t.out(), t.IsVariadic(), flag)
}

// *Signature specific methods

func (t *Signature) IsVariadic() bool {
	return t.rtype.flags&flagVariadic != 0
}

func (t *Signature) NumIn() int {
	return int(t.extra.n1)
}

func (t *Signature) NumOut() int {
	return int(t.extra.n2)
}

func (t *Signature) In(i int) Type {
	return t.in()[i]
}

func (t *Signature) Out(i int) Type {
	return t.out()[i]
}

func (t *Signature) in() []Type {
	extra := t.extra
	return extra.types[0:extra.n1]
}

func (t *Signature) out() []Type {
	extra := t.extra
	return extra.types[extra.n1:]
}

var sigMap = map[interface{}]*Signature{}

// create a new Signature type
func NewSignature(in []Type, out []Type, variadic bool) *Signature {
	key, types := makeSigKey(in, out, variadic)
	t := sigMap[key]
	if t != nil {
		return t
	}
	flag := flagsAnd(in) & flagsAnd(out) & flagComplete
	if variadic {
		flag |= flagVariadic
	}
	t = &Signature{
		rtype: Complete{
			size:  archSizeBytes,
			flags: flag | flagNotComparable,
			kind:  FuncKind,
			str:   makeSignatureString(in, out, variadic, shortPkgName),
		},
		extra: extra{
			n1:    uint32(len(in)),
			n2:    uint32(len(out)),
			types: types,
		},
	}
	t.rtype.typ = t
	t.rtype.extra = &t.extra
	sigMap[key] = t
	return t
}

type (
	sigKey0 = struct{}
	sigKey2 = struct {
		n1, n2 uint16
		inout  [2]Type
	}
	sigKey4 = struct {
		n1, n2 uint16
		inout  [4]Type
	}
	sigKey16 = struct {
		n1, n2 uint16
		inout  [16]Type
	}
	sigKey64 = struct {
		n1, n2 uint16
		inout  [64]Type
	}
	sigKey256 = struct {
		n1, n2 uint16
		inout  [256]Type
	}
	sigKey1k = struct {
		n1, n2 uint16
		inout  [1024]Type
	}
	sigKey4k = struct {
		n1, n2 uint16
		inout  [4096]Type
	}
	sigKey16k = struct {
		n1, n2 uint16
		inout  [16384]Type
	}
	sigKey64k = struct {
		n1, n2 uint16
		inout  [65536]Type
	}
)

func makeSigKey(in []Type, out []Type, variadic bool) (ret interface{}, types []Type) {
	n1, n2 := uint(len(in)), uint(len(out))
	n := n1 + n2
	const maxn = 65536
	if n1 >= maxn || n2 >= maxn/2 || n > maxn {
		panic("NewSignature: too many function parameters and results")
	}
	if variadic {
		n2 |= maxn / 2
	}
	if n == 0 {
		ret = sigKey0{}
	} else if n <= 2 {
		key := sigKey2{n1: uint16(n1), n2: uint16(n2)}
		types = fillSigKey(key.inout[:], in, out)
		ret = key // copies key, must be done after fillSigKey
	} else if n <= 4 {
		key := sigKey4{n1: uint16(n1), n2: uint16(n2)}
		types = fillSigKey(key.inout[:], in, out)
		ret = key
	} else if n <= 16 {
		key := sigKey16{n1: uint16(n1), n2: uint16(n2)}
		types = fillSigKey(key.inout[:], in, out)
		ret = key
	} else if n <= 256 {
		key := sigKey256{n1: uint16(n1), n2: uint16(n2)}
		types = fillSigKey(key.inout[:], in, out)
		ret = key
	} else if n <= 1024 {
		key := sigKey1k{n1: uint16(n1), n2: uint16(n2)}
		types = fillSigKey(key.inout[:], in, out)
		ret = key
	} else if n <= 4096 {
		key := sigKey4k{n1: uint16(n1), n2: uint16(n2)}
		types = fillSigKey(key.inout[:], in, out)
		ret = key
	} else if n <= 16384 {
		key := sigKey16k{n1: uint16(n1), n2: uint16(n2)}
		types = fillSigKey(key.inout[:], in, out)
		ret = key
	} else {
		key := sigKey64k{n1: uint16(n1), n2: uint16(n2)}
		types = fillSigKey(key.inout[:], in, out)
		ret = key
	}
	return ret, types
}

func fillSigKey(dst, in, out []Type) []Type {
	n1, n2 := len(in), len(out)
	dst = dst[:n1+n2]
	copy(dst[:n1], in)
	copy(dst[n1:], out)
	return dst
}

func makeSignatureString(in []Type, out []Type, variadic bool, flag verbose) string {
	var b strings.Builder
	b.WriteString("func")
	writeSignatureTo(&b, in, out, variadic, flag)
	return b.String()
}

// does NOT write "func" prefix
func writeSignatureTo(b *strings.Builder, in []Type, out []Type, variadic bool, flag verbose) {
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

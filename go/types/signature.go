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
	isSignature struct{} // occupies zero bytes
	rtype       Complete
	extra       extra
}

// *Signature implements Type

func (t *Signature) String() string {
	return t.rtype.str
}

func (t *Signature) Underlying() Type {
	return t
}

func (t *Signature) common() *Complete {
	return &t.rtype
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
	extra := t.extra
	return extra.types[0:extra.n1][i]
}

func (t *Signature) Out(i int) Type {
	extra := t.extra
	return extra.types[extra.n1:][i]
}

var signatureMap = map[interface{}]*Signature{}

// create a new Signature type
func NewSignature(in []Type, out []Type, variadic bool) *Signature {
	key, types := makeSignatureKey(in, out, variadic)
	t := signatureMap[key]
	if t != nil {
		return t
	}
	var isVariadic flags
	if variadic {
		isVariadic = flagVariadic
	}
	t = &Signature{
		rtype: Complete{
			size:  archSizeBytes,
			flags: (flagsAnd(in) & flagsAnd(out) & flagComplete) | isVariadic,
			kind:  FuncKind,
			str:   makeSignatureString(in, out, variadic),
		},
		extra: extra{
			n1:    uint32(len(in)),
			n2:    uint32(len(out)),
			types: types,
		},
	}
	t.rtype.typ = t
	t.rtype.extra = &t.extra
	signatureMap[key] = t
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

func makeSignatureKey(in []Type, out []Type, variadic bool) (ret interface{}, types []Type) {
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
		ret = funcKey0{}
	} else if n <= 2 {
		key := funcKey2{n1: uint16(n1), n2: uint16(n2)}
		types = fillFuncKey(key.inout[:], in, out)
		ret = key // copies key, must be done after fillFuncKey
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

func makeSignatureString(in []Type, out []Type, variadic bool) string {
	var b strings.Builder
	b.WriteString("func(")
	for i, t := range in {
		if i != 0 {
			b.WriteString(", ")
		}
		if i+1 == len(in) && variadic {
			b.WriteString("...")
		}
		b.WriteString(t.String())
	}
	b.WriteByte(')')

	switch len(out) {
	case 0:
	case 1:
		b.WriteByte(' ')
		b.WriteString(out[0].String())
	default:
		b.WriteString(" (")
		for i, t := range out {
			if i != 0 {
				b.WriteString(", ")
			}
			b.WriteString(t.String())
		}
		b.WriteByte(')')
	}
	return b.String()
}

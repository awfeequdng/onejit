/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * interface.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"sort"
	"strings"
)

type Interface struct {
	_     [0]*Interface // occupies zero bytes
	rtype Complete
	extra extra
}

// *Interface implements Type

func (t *Interface) String() string {
	return t.rtype.str
}

func (t *Interface) Underlying() Type {
	return t
}

func (t *Interface) common() *Complete {
	return &t.rtype
}

// *Interface specific method

// return number of embedded interfaces.
func (t *Interface) NumEmbedded() int {
	return len(t.extra.types)
}

// return number of declared method.
// Ignores method from embedded interfaces
func (t *Interface) NumMethod() int {
	return len(t.extra.methods)
}

// return i-th embedded interface.
func (t *Interface) Embedded(i int) Type {
	return t.extra.types[i]
}

// return i-th declared method.
// Ignores method from embedded interfaces
func (t *Interface) Method(i int) Method {
	return t.extra.methods[i]
}

var interfaceMap = map[interface{}]*Interface{}

// create a new Interface type
func NewInterface(embedded []Type, method []Method) *Interface {
	key, embedded, method := makeInterfaceKey(embedded, method)
	t := interfaceMap[key]
	if t != nil {
		return t
	}
	flag := flagsAnd(embedded) & flagsAndMethod(method) & flagComplete
	t = &Interface{
		rtype: Complete{
			methods: &method,
			size:    2 * archSizeBytes,
			flags:   flag | flagComparable,
			kind:    InterfaceKind,
			str:     makeInterfaceString(embedded, method),
		},
		extra: extra{
			types:   embedded,
			methods: method,
		},
	}
	t.rtype.methods = &t.extra.methods
	t.rtype.typ = t
	t.rtype.extra = &t.extra
	interfaceMap[key] = t
	return t
}

func makeInterfaceString(embedded []Type, method []Method) string {
	var b strings.Builder
	b.WriteString("interface {")
	for i, t := range embedded {
		if i == 0 {
			b.WriteByte(' ')
		} else {
			b.WriteString("; ")
		}
		b.WriteString(t.String())
	}
	for i := range method {
		if i == 0 && len(embedded) == 0 {
			b.WriteByte(' ')
		} else {
			b.WriteString("; ")
		}
		method[i].writeTo(&b)
	}
	if len(embedded) == 0 && len(method) == 0 {
		b.WriteByte('}')
	} else {
		b.WriteString(" }")
	}
	return b.String()
}

type (
	interfaceKey0 = struct{}
	interfaceKey2 = struct {
		embedded [2]Type
		method   [2]Method
	}
	interfaceKey4 = struct {
		embedded [4]Type
		method   [4]Method
	}
	interfaceKey16 = struct {
		embedded [16]Type
		method   [16]Method
	}
	interfaceKey64 = struct {
		embedded [64]Type
		method   [64]Method
	}
	interfaceKey256 = struct {
		embedded [256]Type
		method   [256]Method
	}
	interfaceKey1k = struct {
		embedded [1024]Type
		method   [1024]Method
	}
	interfaceKey4k = struct {
		embedded [4096]Type
		method   [4096]Method
	}
	interfaceKey16k = struct {
		embedded [16384]Type
		method   [16384]Method
	}
	interfaceKey64k = struct {
		embedded [65536]Type
		method   [65536]Method
	}
)

func makeInterfaceKey(embedded []Type, method []Method) (ret interface{}, rembedded []Type, rmethod []Method) {
	n1, n2 := len(embedded), len(method)
	const maxn = 65536
	if n1 > maxn {
		panic("NewInterface: too many embedded interfaces")
	} else if n2 > maxn {
		panic("NewInterface: too many method")
	}
	n := n2
	if n < n1 {
		n = n1
	}
	if n == 0 {
		ret = interfaceKey0{}
	} else if n <= 2 {
		key := interfaceKey2{}
		rembedded, rmethod = fillInterfaceKey(key.embedded[:], key.method[:], embedded, method)
		ret = key // copies key, must be done after fillInterfaceKey
	} else if n <= 4 {
		key := interfaceKey4{}
		rembedded, rmethod = fillInterfaceKey(key.embedded[:], key.method[:], embedded, method)
		ret = key
	} else if n <= 16 {
		key := interfaceKey16{}
		rembedded, rmethod = fillInterfaceKey(key.embedded[:], key.method[:], embedded, method)
		ret = key
	} else if n <= 64 {
		key := interfaceKey64{}
		rembedded, rmethod = fillInterfaceKey(key.embedded[:], key.method[:], embedded, method)
		ret = key
	} else if n <= 256 {
		key := interfaceKey256{}
		rembedded, rmethod = fillInterfaceKey(key.embedded[:], key.method[:], embedded, method)
		ret = key
	} else if n <= 1024 {
		key := interfaceKey1k{}
		rembedded, rmethod = fillInterfaceKey(key.embedded[:], key.method[:], embedded, method)
		ret = key
	} else if n <= 4096 {
		key := interfaceKey4k{}
		rembedded, rmethod = fillInterfaceKey(key.embedded[:], key.method[:], embedded, method)
		ret = key
	} else if n <= 16384 {
		key := interfaceKey16k{}
		rembedded, rmethod = fillInterfaceKey(key.embedded[:], key.method[:], embedded, method)
		ret = key
	} else if n <= 65536 {
		key := interfaceKey64k{}
		rembedded, rmethod = fillInterfaceKey(key.embedded[:], key.method[:], embedded, method)
		ret = key
	}
	return ret, rembedded, rmethod

}

func fillInterfaceKey(embout []Type, mtdout []Method, emb []Type, mtd []Method) ([]Type, []Method) {
	return fillInterfaceKeyTypes(embout, emb), fillInterfaceKeyMethods(mtdout, mtd)
}

func fillInterfaceKeyTypes(out []Type, in []Type) []Type {
	out = out[:len(in)]
	copy(out, in)
	sortTypes(out)
	for _, t := range out {
		if t == nil || t.common().kind != Invalid && t.common().kind != InterfaceKind {
			panic("NewInterface: invalid embedded interface type")
		}
	}
	return out
}

func fillInterfaceKeyMethods(out []Method, in []Method) []Method {
	out = out[:len(in)]
	copy(out, in)
	sortMethods(out)
	for i := range out {
		out[i].Index = i
		if _, ok := out[i].Type.(*Func); !ok {
			panic("NewInterface: invalid method type")
		}
	}
	return out
}

// sortableTypes

type sortableTypes []Type

func (ts sortableTypes) Len() int {
	return len(ts)
}

func (ts sortableTypes) Less(i, j int) bool {
	return ts[i].String() < ts[j].String()
}

func (ts sortableTypes) Swap(i, j int) {
	ts[i], ts[j] = ts[j], ts[i]
}

func sortTypes(ts []Type) {
	sort.Sort(sortableTypes(ts))
}

// sortableMethods

type sortableMethods []Method

func (ms sortableMethods) Len() int {
	return len(ms)
}

func (ms sortableMethods) Less(i, j int) bool {
	name1, pkg1 := ms[i].Name, ms[i].PkgPath
	name2, pkg2 := ms[j].Name, ms[j].PkgPath
	return pkg1 < pkg2 || (pkg1 == pkg2 && name1 < name2)
}

func (ms sortableMethods) Swap(i, j int) {
	ms[i], ms[j] = ms[j], ms[i]
}

func sortMethods(ms []Method) {
	sort.Sort(sortableMethods(ms))
}

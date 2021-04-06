/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_basic_test.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"testing"

	"github.com/cosmos72/onejit/go/arch"
)

func TestBasic(test *testing.T) {
	for _, os := range [...]arch.Os{arch.Linux, arch.Windows, arch.OsOther} {
		for _, arc := range [...]arch.Arch{
			arch.I386, arch.Amd64, arch.Arm, arch.Arm64, arch.ArchOther32bit, arch.ArchOther64bit,
		} {
			arch.SetTarget(os, arc)
			for i, basic := range BasicTypes() {
				if basic == nil {
					continue
				}
				kind := Kind(i)
				if basic.Kind() != kind {
					test.Errorf("basic.Kind()\t= %v,\texpecting %v", basic.Kind(), kind)
				}
				if basic.String() != kind.String() {
					test.Errorf("basic.String()\t= %v,\texpecting %v", basic.String(), kind.String())
				}
			}
		}
	}
	arch.SetTarget(arch.OsAuto, arch.ArchAuto)
}

func TestFunc(test *testing.T) {
	for _, b1 := range BasicTypes() {
		if b1 == nil || b1.Type() == nil {
			continue
		}
		arg1 := b1.Type()
		for _, b2 := range BasicTypes() {
			if b2 == nil || b2.Type() == nil {
				continue
			}
			arg2 := b2.Type()
			t := NewFunc([]Type{arg1, arg2}, []Type{arg2, arg1}, true)
			if expected := "func(" + arg1.String() + ", ..." + arg2.String() +
				") (" + arg2.String() + ", " + arg1.String() + ")"; t.String() != expected {

				test.Errorf("t.String()\t= %v,\texpecting %v", t.String(), expected)
			}

			if actual, expected := t.common().Size(), sizeOfPtr(); actual != expected {
				test.Errorf("t.common().Size()\t= %v,\texpecting %v", actual, expected)
			}

			if actual, expected := t.In(1), arg2; actual != expected {
				test.Errorf("t.In(1)\t= %v,\texpecting %v", actual, expected)
			}
			if actual, expected := t.Out(1), arg1; actual != expected {
				test.Errorf("t.Out(1)\t= %v,\texpecting %v", actual, expected)
			}

			tagain := NewFunc([]Type{arg1, arg2}, []Type{arg2, arg1}, true)
			if t != tagain {
				test.Errorf("NewFunc({%v,%v}, {%v,%v}) produced non-identical types %p and %p",
					arg1, arg2, arg2, arg1, t, tagain)
			}
		}
	}
}

func TestInterface(test *testing.T) {
	pkgPath := "github.com/cosmos72/onejit/go/types/test"

	bbool := BasicType(Bool).Type()
	for _, bc := range BasicTypes() {
		if bc == nil || bc.Type() == nil {
			continue
		}
		b := bc.Type()
		fun := NewFunc([]Type{b, b}, []Type{bbool}, false)
		methods := []Method{{
			Type:    fun,
			Name:    "less",
			PkgPath: pkgPath,
		}}
		t := NewInterface(nil, methods...)
		if actual, expected := t.String(), "interface { "+pkgPath+".less("+b.String()+", "+b.String()+") bool }"; actual != expected {
			test.Errorf("t.String()\t= %v,\texpecting %v", actual, expected)
		}
		tagain := NewInterface(nil, methods...)
		if t != tagain {
			test.Errorf("NewInterface() produced non-identical types %p and %p", t, tagain)
		}

	}
}

func TestInterfaceEmbedded(test *testing.T) {
	for _, bc := range BasicTypes() {
		if bc == nil || bc.Type() == nil {
			continue
		}
		b := bc.Type()
		fun := NewFunc(nil, []Type{b}, false)
		methods := []Method{{
			Type: fun,
			Name: "Value",
		}}
		t := NewInterface(nil, methods...)
		if actual, expected := t.String(), "interface { Value() "+b.String()+" }"; actual != expected {
			test.Errorf("t.String()\t= %v,\texpecting %v", actual, expected)
		}
		tagain := NewInterface([]Type{t}, methods...)
		if t != tagain {
			test.Errorf("NewInterface() produced non-identical types %p and %p", t, tagain)
		}

	}
}

func TestMap(test *testing.T) {
	for _, bkey := range BasicTypes() {
		if bkey == nil || bkey.Type() == nil {
			continue
		}
		key := bkey.Type()
		for _, belem := range BasicTypes() {
			if belem == nil || belem.Type() == nil {
				continue
			}
			elem := belem.Type()
			t := NewMap(key, elem)
			if t.Key() != key {
				test.Errorf("t.Key()\t= %v,\texpecting %v", t.Key(), key)
			}
			if t.Elem() != elem {
				test.Errorf("t.Elem()\t= %v,\texpecting %v", t.Elem(), elem)
			}

			if expected := "map[" + key.String() + "]" + elem.String(); t.String() != expected {
				test.Errorf("t.String()\t= %v,\texpecting %v", t.String(), expected)
			}

			if actual, expected := t.common().Size(), sizeOfPtr(); actual != expected {
				test.Errorf("t.common().Size()\t= %v,\texpecting %v", actual, expected)
			}
			tagain := NewMap(key, elem)
			if t != tagain {
				test.Errorf("NewMap(%v, %v) produced non-identical types %p and %p", key, elem, t, tagain)
			}
		}
	}
}

func TestNamed(test *testing.T) {
	pkgPath := "github.com/cosmos72/onejit/go/types/test"
	for _, bc := range BasicTypes() {
		if bc == nil || bc.Type() == nil {
			continue
		}
		basic := bc.Type().(*Basic)
		name := basic.String() + "_"
		t := NewNamed(name, pkgPath)
		if actual, expected := t.String(), pkgPath+"."+name; actual != expected {
			test.Errorf("t.String()\t= %v,\texpecting %v", actual, expected)
		}
		if actual, expected := t.Underlying(), Type(nil); actual != expected {
			test.Errorf("t.Underlying()\t= %v,\texpecting %v", actual, expected)
		}
		if actual, expected := t.common().Kind(), Invalid; actual != expected {
			test.Errorf("t.common().Kind()\t= %v,\texpecting %v", actual, expected)
		}
		if actual, expected := t.Name(), name; actual != expected {
			test.Errorf("t.Name()\t= %v,\texpecting %v", actual, expected)
		}
		if actual, expected := t.PkgPath(), pkgPath; actual != expected {
			test.Errorf("t.PkgPath()\t= %v,\texpecting %v", actual, expected)
		}
		t.SetUnderlying(basic)
		if actual, expected := t.Underlying(), basic; actual != expected {
			test.Errorf("t.Underlying()\t= %v,\texpecting %v", actual, expected)
		}
		if actual, expected := t.common().Kind(), basic.Kind(); actual != expected {
			test.Errorf("t.common().Kind()\t= %v,\texpecting %v", actual, expected)
		}
		if actual, expected := t.common().Size(), basic.Size(); actual != expected {
			test.Errorf("t.common().Size()\t= %v,\texpecting %v", actual, expected)
		}
		m := Method{
			Type: NewFunc([]Type{t}, nil, false),
			Name: "bar",
		}
		if actual, expected := t.NumMethod(), 0; actual != expected {
			test.Errorf("t.NumMethod()\t= %v,\texpecting %v", actual, expected)
		}
		t.AddMethod(&m)
		if actual, expected := t.NumMethod(), 1; actual != expected {
			test.Errorf("t.NumMethod()\t= %v,\texpecting %v", actual, expected)
		}
		if actual, expected := t.Method(0), m; actual != expected {
			test.Errorf("t.Method(0)\t= %v,\texpecting %v", actual, expected)
		}

		tagain := NewNamed(name, pkgPath)
		if t == tagain {
			test.Errorf("NewNamed(%q, %q) produced identical types %p and %p", name, pkgPath, t, tagain)
		}
	}
}

func TestPointer(test *testing.T) {
	for _, bc := range BasicTypes() {
		if bc == nil || bc.Type() == nil {
			continue
		}
		basic := bc.Type()
		t := NewPointer(basic)
		tt := NewPointer(t)
		if t.Elem() != basic {
			test.Errorf("t.Elem()\t= %v,\texpecting %v", t.Elem(), basic)
		}
		if tt.Elem() != t {
			test.Errorf("tt.Elem()\t= %v,\texpecting %v", tt.Elem(), t)
		}

		if expected := "*" + basic.String(); t.String() != expected {
			test.Errorf("t.String()\t= %v,\texpecting %v", t.String(), expected)
		}
		if expected := "**" + basic.String(); tt.String() != expected {
			test.Errorf("tt.String()\t= %v,\texpecting %v", tt.String(), expected)
		}

		if actual, expected := t.common().Size(), sizeOfPtr(); actual != expected {
			test.Errorf("t.common().Size()\t= %v,\texpecting %v", actual, expected)
		}
		if actual, expected := tt.common().Size(), sizeOfPtr(); actual != expected {
			test.Errorf("tt.common().Size()\t= %v,\texpecting %v", actual, expected)
		}
		tagain := NewPointer(basic)
		ttagain := NewPointer(tagain)
		if t != tagain {
			test.Errorf("NewPointer(%v) produced non-identical types %p and %p", basic, t, tagain)
		}
		if tt != ttagain {
			test.Errorf("NewPointer(%v) produced non-identical types %p and %p", t, tt, ttagain)
		}
	}
}

func TestSlice(test *testing.T) {
	for _, bc := range BasicTypes() {
		if bc == nil || bc.Type() == nil {
			continue
		}
		basic := bc.Type()
		t := NewSlice(basic)
		tt := NewSlice(t)
		if t.Elem() != basic {
			test.Errorf("t.Elem()\t= %v,\texpecting %v", t.Elem(), basic)
		}
		if tt.Elem() != t {
			test.Errorf("tt.Elem()\t= %v,\texpecting %v", tt.Elem(), t)
		}

		if expected := "[]" + basic.String(); t.String() != expected {
			test.Errorf("t.String()\t= %v,\texpecting %v", t.String(), expected)
		}
		if expected := "[][]" + basic.String(); tt.String() != expected {
			test.Errorf("tt.String()\t= %v,\texpecting %v", tt.String(), expected)
		}

		if actual, expected := t.common().Size(), 3*sizeOfPtr(); actual != expected {
			test.Errorf("t.common().Size()\t= %v,\texpecting %v", actual, expected)
		}
		if actual, expected := tt.common().Size(), 3*sizeOfPtr(); actual != expected {
			test.Errorf("tt.common().Size()\t= %v,\texpecting %v", actual, expected)
		}

		tagain := NewSlice(basic)
		ttagain := NewSlice(tagain)
		if t != tagain {
			test.Errorf("NewSlice(%v) produced non-identical types %p and %p", basic, t, tagain)
		}
		if tt != ttagain {
			test.Errorf("NewSlice(%v) produced non-identical types %p and %p", t, tt, ttagain)
		}
	}
}

func TestStruct(test *testing.T) {
	fields := make([]Field, 2)
	for _, bf0 := range BasicTypes() {
		if bf0 == nil || bf0.Type() == nil {
			continue
		}
		f0 := bf0.Type().(*Basic)
		fields[0] = Field{
			Type:     f0,
			Name:     f0.Name(),
			Offset:   unknownSize,
			Index:    0,
			Embedded: true,
		}
		for _, bf1 := range BasicTypes() {
			if bf1 == nil || bf1.Type() == nil {
				continue
			}
			f1 := bf1.Type().(*Basic)
			fields[1] = Field{
				Type:     f1,
				Name:     f1.Name(),
				Offset:   unknownSize,
				Index:    1,
				Embedded: true,
			}

			t := NewStruct(fields...)
			if expected := "struct { " + f0.String() + "; " + f1.String() + " }"; t.String() != expected {
				test.Errorf("t.String()\t= %v,\texpecting %v", t.String(), expected)
			}

			if actual, expected := t.Field(0), fields[0]; actual != expected {
				test.Errorf("t.Field(0)\t= %v,\texpecting %v", actual, expected)
			}
			if actual, expected := t.Field(1), fields[1]; actual != expected {
				test.Errorf("t.Field(1)\t= %v,\texpecting %v", actual, expected)
			}

			tagain := NewStruct(fields...)
			if t != tagain {
				test.Errorf("NewStruct(%v) produced non-identical types %p and %p",
					fields, t, tagain)
			}
		}
	}
}

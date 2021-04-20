/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_test.go
 *
 *  Created on: Apr 14, 2021
 *      Author: Massimiliano Ghilardi
 */

package constant

import (
	"math/big"
	"testing"

	"github.com/cosmos72/onejit/go/token"
)

func TestBool(t *testing.T) {
	vfalse, _ := Make(Bool, false)
	vtrue, _ := Make(Bool, true)
	if x := vfalse.Kind(); x != Bool {
		t.Errorf("vfalse.Kind() = %v", x)
	}
	if x := vtrue.Kind(); x != Bool {
		t.Errorf("vtrue.Kind() = %v", x)
	}
	if x := vfalse.Bool(); x != false {
		t.Errorf("vfalse.Bool() = %v", x)
	}
	if x := vtrue.Bool(); x != true {
		t.Errorf("vtrue.Bool() = %v", x)
	}
	if x, err := vfalse.To(token.Bool); x.Kind() != token.Bool || x.Bool() != false || err != nil {
		t.Errorf("vfalse.To(token.Bool) = %v", x)
	}
	if x, err := vtrue.To(token.Bool); x.Kind() != token.Bool || x.Bool() != true || err != nil {
		t.Errorf("vtrue.To(token.Bool) = %v", x)
	}
}

func TestInt(t *testing.T) {
	b1 := big.NewInt(1)
	b2to100 := new(big.Int)
	b2to100.Lsh(b1, 100) // 1<<100 i.e. pow(2, 100)

	v2to7, _ := Make(Int, int64(128))
	v2to100, _ := Make(UntypedInt, b2to100)

	if x := v2to7.Kind(); x != Int {
		t.Errorf("v2to7.Kind() = %v", x)
	}
	if x := v2to100.Kind(); x != UntypedInt {
		t.Errorf("v2to100.Kind() = %v", x)
	}
	if x, exact := v2to7.Int64(); x != int64(128) || !exact {
		t.Errorf("v2to7.Int64() = %v, %v", x, exact)
	}
	if x, err := v2to100.Int64(); err {
		t.Errorf("v2to100.Int64() = %v, %v", x, err)
	}
	if x, err := v2to7.To(token.Int8); err == nil {
		t.Errorf("v2to7.To(token.Int8) = %v, %v", x, err)
	}
	if x, err := v2to7.To(token.Uint8); err != nil {
		t.Errorf("v2to7.To(token.Uint8) = %v, %v", x, err)
	} else if i, exact := x.Uint64(); i != uint64(128) || !exact {
		t.Errorf("v2to7.To(token.Uint8).Uint64() = %v, %v", i, exact)
	}
	if x, err := v2to100.To(token.Int64); err == nil {
		t.Errorf("v2to100.To(token.Int64) = %v, %v", x, err)
	}
	if x, y := v2to100.String(), "{untyped.Int 1267650600228229401496703205376}"; x != y {
		t.Errorf("v2to100.String() = %s", x)
	}
}

func TestIntShift(t *testing.T) {
	b1 := big.NewInt(1)
	b2 := new(big.Int)
	b2.Lsh(b1, 100)
	v1, err := Make(Int, b1)
	if err != nil {
		t.Error(err)
	}
	v2, err := Make(UntypedInt, b2)
	if err != nil {
		t.Error(err)
	}
	v2, err = v2.To(UntypedFloat)
	if err != nil {
		t.Error(err)
	}
	v3, err := Shift(v2, token.SHL, v1)
	if actual, expected := v3.String(), "{untyped.Int 2535301200456458802993406410752}"; actual != expected {
		t.Errorf("v3.String() = %v", actual)
	}
}

func TestRune(t *testing.T) {
	veur, _ := Make(UntypedRune, int64('\u20ac'))
	if x := veur.Kind(); x != UntypedRune {
		t.Errorf("veur.Kind() = %v", x)
	}
	if x, exact := veur.Int64(); x != int64('\u20ac') || !exact {
		t.Errorf("veur.Int64() = %v, %v", x, exact)
	}
	if x, err := veur.To(token.Int8); err == nil {
		t.Errorf("veur.To(token.Int8) = %v, %v", x, err)
	}
	if x, err := veur.To(token.Uint8); err == nil {
		t.Errorf("veur.To(token.Uint8) = %v, %v", x, err)
	}
	if x, err := veur.To(token.Int16); err != nil {
		t.Errorf("veur.To(token.Int16) = %v, %v", x, err)
	} else if i, exact := x.Int64(); i != int64('\u20ac') || !exact {
		t.Errorf("veur.To(token.Int16).Int64() = %v, %v", i, exact)
	}
	if x, err := veur.To(token.Int32); err != nil {
		t.Errorf("veur.To(token.Int32) = %v, %v", x, err)
	} else if i, exact := x.Int64(); i != int64('\u20ac') || !exact {
		t.Errorf("veur.To(token.Int32).Int64() = %v, %v", x, err)
	}
	if x, y := veur.String(), `{untyped.Rune '\u20ac'}`; x != y {
		t.Errorf("veur.String() = %s", x)
	}

	vquote, _ := Make(UntypedRune, int64('\''))
	if x := vquote.Kind(); x != UntypedRune {
		t.Errorf("vquote.Kind() = %v", x)
	}
	if x, exact := vquote.Int64(); x != int64('\'') || !exact {
		t.Errorf("vquote.Int64() = %v, %v", x, exact)
	}
	if x, err := vquote.To(token.Int8); err != nil {
		t.Errorf("vquote.To(token.Int8) = %v, %v", x, err)
	} else if i, exact := x.Int64(); i != int64('\'') || !exact {
		t.Errorf("vquote.To(token.Int8).Int64() = %v, %v", i, exact)
	}
	if x, y := vquote.String(), `{untyped.Rune '\''}`; x != y {
		t.Errorf("vquote.String() = %s", x)
	}
}

func TestFloat(t *testing.T) {
	f1, f2 := float32(8.5), float64(1e308)

	v1, _ := Make(Float32, f1)
	v2, _ := Make(UntypedFloat, f2)

	if x := v1.Kind(); x != Float32 {
		t.Errorf("v1.Kind() = %v", x)
	}
	if x := v2.Kind(); x != UntypedFloat {
		t.Errorf("v2.Kind() = %v", x)
	}
	if x, exact := v1.Float32(); x != f1 || !exact {
		t.Errorf("v1.Float32() = %v, %v", x, exact)
	}
	if x, exact := v2.Float64(); x != f2 || !exact {
		t.Errorf("v2.Float64() = %v, %v", x, exact)
	}
}

func TestComplex(t *testing.T) {
	fre, fim := float32(8.5), float64(1e308)

	vre, _ := Make(UntypedFloat, fre)
	vim, _ := Make(UntypedFloat, fim)
	v, _ := BinaryOp(vre, token.ADD, MakeImag(vim))

	if x := v.Kind(); x != UntypedComplex {
		t.Errorf("v.Kind() = %v", x)
	}
	if eq, err := Compare(v.Real(), token.EQL, vre); err != nil || !eq {
		t.Errorf("v.Real() = %v -- expecting %v", v.Real(), vre)
	}
}

func TestString(t *testing.T) {
	v1, _ := Make(UntypedString, "foo")
	v2, _ := Make(String, "bar")
	v, _ := BinaryOp(v1, token.ADD, v2)

	if x := v1.Kind(); x != UntypedString {
		t.Errorf("v1.Kind() = %v", x)
	}
	if x := v2.Kind(); x != String {
		t.Errorf("v2.Kind() = %v", x)
	}
	if x := v.Kind(); x != String {
		t.Errorf("v.Kind() = %v", x)
	}
	if actual, expected := v.StringVal(), "foobar"; actual != expected {
		t.Errorf("v.StringVal() = %v", actual)
	}
}

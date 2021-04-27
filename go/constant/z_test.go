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

	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

func TestBool(t *testing.T) {
	tbool := types.BasicType(Bool)
	vfalse := Make(tbool, false)
	vtrue := Make(tbool, true)
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
	if x := vfalse.To(tbool); x.Type() != tbool || !x.IsValid() || x.Bool() != false {
		t.Errorf("vfalse.To(Bool) = %v", x)
	}
	if x := vtrue.To(tbool); x.Type() != tbool || !x.IsValid() || x.Bool() != true {
		t.Errorf("vtrue.To(Bool) = %v", x)
	}
}

func TestInt(t *testing.T) {
	b1 := big.NewInt(1)
	b2to100 := new(big.Int)
	b2to100.Lsh(b1, 100) // 1<<100 i.e. pow(2, 100)

	tint := types.BasicType(Int)
	tint8 := types.BasicType(Int8)
	tuint8 := types.BasicType(Uint8)
	tuint64 := types.BasicType(Uint64)
	tuntypedint := types.BasicType(UntypedInt)

	v2to7 := Make(tint, int64(128))
	v2to100 := Make(tuntypedint, b2to100)

	if x := v2to7.Kind(); x != Int {
		t.Errorf("v2to7.Kind() = %v", x)
	}
	if x := v2to100.Kind(); x != UntypedInt {
		t.Errorf("v2to100.Kind() = %v", x)
	}
	if x, exact := v2to7.Int64(); x != int64(128) || !exact {
		t.Errorf("v2to7.Int64() = %v, %v", x, exact)
	}
	if x, exact := v2to100.Int64(); exact {
		t.Errorf("v2to100.Int64() = %v, %v", x, exact)
	}

	if x := v2to7.To(tint8); x.IsValid() {
		t.Errorf("v2to7.To(int8) = %v", x)
	}
	if x := v2to7.To(tuint8); !x.IsValid() {
		t.Errorf("v2to7.To(tuint8) = %v", x)
	} else if i, exact := x.Uint64(); i != uint64(128) || !exact {
		t.Errorf("v2to7.To(tuint8).Uint64() = %v, %v", i, exact)
	}
	if x := v2to100.To(tuint64); x.IsValid() {
		t.Errorf("v2to100.To(tuint64) = %v", x)
	}
	if x, y := v2to100.String(), "{untyped.Int 1267650600228229401496703205376}"; x != y {
		t.Errorf("v2to100.String() = %s", x)
	}
}

func TestIntShift(t *testing.T) {
	b1 := big.NewInt(1)
	b2 := new(big.Int)
	b2.Lsh(b1, 100)
	v1 := Make(types.BasicType(Int), b1)
	if !v1.IsValid() {
		t.Error(v1.Err())
	}
	v2 := Make(types.BasicType(UntypedInt), b2)
	if !v2.IsValid() {
		t.Error(v2.Err())
	}
	v2 = v2.To(types.BasicType(UntypedFloat))
	if !v2.IsValid() {
		t.Error(v2.Err())
	}
	v3 := BinaryOp(v2, token.SHL, v1)
	if actual, expected := v3.String(), "{untyped.Int 2535301200456458802993406410752}"; actual != expected {
		t.Errorf("v3.String() = %v", actual)
	}
}

func TestRune(t *testing.T) {
	tuntypedrune := types.BasicType(UntypedRune)
	veur := Make(tuntypedrune, int64('\u20ac'))
	if x := veur.Kind(); x != UntypedRune {
		t.Errorf("veur.Kind() = %v", x)
	}
	if x, exact := veur.Int64(); x != int64('\u20ac') || !exact {
		t.Errorf("veur.Int64() = %v, %v", x, exact)
	}
	if x := veur.To(types.BasicType(Int8)); x.IsValid() {
		t.Errorf("veur.To(int8) = %v", x)
	}
	if x := veur.To(types.BasicType(Uint8)); x.IsValid() {
		t.Errorf("veur.To(uint8) = %v", x)
	}
	if x := veur.To(types.BasicType(Int16)); !x.IsValid() {
		t.Errorf("veur.To(int16) = %v", x)
	} else if i, exact := x.Int64(); i != int64('\u20ac') || !exact {
		t.Errorf("veur.To(int16).Int64() = %v, %v", i, exact)
	}
	if x := veur.To(types.BasicType(Int32)); !x.IsValid() {
		t.Errorf("veur.To(int32) = %v", x)
	} else if i, exact := x.Int64(); i != int64('\u20ac') || !exact {
		t.Errorf("veur.To(int32).Int64() = %v, %v", x, exact)
	}
	if x, y := veur.String(), `{untyped.Rune '\u20ac'}`; x != y {
		t.Errorf("veur.String() = %s", x)
	}

	vquote := Make(tuntypedrune, int64('\''))
	if x := vquote.Kind(); x != UntypedRune {
		t.Errorf("vquote.Kind() = %v", x)
	}
	if x, exact := vquote.Int64(); x != int64('\'') || !exact {
		t.Errorf("vquote.Int64() = %v, %v", x, exact)
	}
	if x := vquote.To(types.BasicType(Int8)); !x.IsValid() {
		t.Errorf("vquote.To(int8) = %v", x)
	} else if i, exact := x.Int64(); i != int64('\'') || !exact {
		t.Errorf("vquote.To(int8).Int64() = %v, %v", i, exact)
	}
	if x, y := vquote.String(), `{untyped.Rune '\''}`; x != y {
		t.Errorf("vquote.String() = %s", x)
	}
}

func TestFloat(t *testing.T) {
	f1, f2 := float32(8.5), float64(1e308)

	v1 := Make(types.BasicType(Float32), f1)
	v2 := Make(types.BasicType(UntypedFloat), f2)

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

	tuntypedfloat := types.BasicType(UntypedFloat)

	vre := Make(tuntypedfloat, fre)
	vim := Make(tuntypedfloat, fim)
	v := BinaryOp(vre, token.ADD, MakeImag(vim))

	if x := v.Kind(); x != UntypedComplex {
		t.Errorf("v.Kind() = %v", x)
	}
	if eq := BinaryOp(v.Real(), token.EQL, vre); !eq.IsValid() || !eq.Bool() {
		t.Errorf("v.Real() = %v -- expecting %v", v.Real(), vre)
	}
}

func TestString(t *testing.T) {
	v1 := Make(types.BasicType(UntypedString), "foo")
	v2 := Make(types.BasicType(String), "bar")
	v := BinaryOp(v1, token.ADD, v2)

	if x := v1.Kind(); x != UntypedString {
		t.Errorf("v1.Kind() = %v", x)
	}
	if x := v2.Kind(); x != String {
		t.Errorf("v2.Kind() = %v", x)
	}
	if x := v.Kind(); x != String {
		t.Errorf("v.Kind() = %v", x)
	}
	{
		expected := "foobar"
		if x, exact := v.StringVal(); !exact || x != expected {
			t.Errorf("v.StringVal() = %v, %v", x, exact)
		}
	}
}

func TestMakeFromLiteral(t *testing.T) {
	type TestCase struct {
		str      string
		tok      token.Token
		val      interface{}
		expected string
	}
	testcases := []TestCase{
		{"1234567890", token.INT, int64(1234567890), "{untyped.Int 1234567890}"},
		{"077", token.INT, int64(077), "{untyped.Int " + strings.UintToString(077) + "}"},
		{"0b01_01_10", token.INT, int64(22), "{untyped.Int 22}"},
		{"0o1_2", token.INT, int64(012), "{untyped.Int " + strings.UintToString(012) + "}"},
		{"0x1234567890_1234567890_12", token.INT, nil, "{untyped.Int 22007822917626582913290258}"},
		{"0x12.45p0", token.FLOAT, nil, "{untyped.Float 18.2695}"},
		{"7i", token.IMAG, nil, "{untyped.Complex (0 + 7i)}"},
		{"'a'", token.CHAR, int64('a'), "{untyped.Rune 'a'}"},
		{"'\u20ac'", token.CHAR, int64('\u20ac'), `{untyped.Rune '\u20ac'}`},
		{"\"foo\"", token.STRING, "foo", `{untyped.String "foo"}`},
		{"\"\U0010ffff\"", token.STRING, "\U0010ffff", `{untyped.String "\U0010ffff"}`},
	}
	for _, testcase := range testcases {
		v := MakeFromLiteral(testcase.str, testcase.tok)
		if actual, expected := v.Value(), testcase.val; expected != nil && actual != expected {
			t.Errorf("v.Value() = %v /*%T*/ -- expecting %v /*%T*/", actual, actual, expected, expected)
		}
		if actual, expected := v.String(), testcase.expected; actual != expected {
			t.Errorf("v.String() = %v -- expecting %v", actual, expected)
		}
	}
}

func TestBinaryOp(t *testing.T) {
	/*
	 * runtime equivalent to:
	 * const c1 = imag(1.0<<100 * 3i)
	 */
	v := BinaryOp(
		BinaryOp(
			Make(types.BasicType(UntypedFloat), 1.0),
			token.SHL,
			Make(types.BasicType(UntypedInt), 100)),
		token.MUL,
		MakeImag(Make(types.BasicType(UntypedInt), 3)),
	).Imag()
	if v.Kind() != UntypedFloat {
		t.Errorf("v = %v", v)
	}
}

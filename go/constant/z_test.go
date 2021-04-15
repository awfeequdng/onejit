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
	vfalse := Make(Bool, false)
	vtrue := Make(Bool, true)
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
	if x, err := vfalse.ToTyped(token.Bool); x != false || err != nil {
		t.Errorf("vfalse.ToTyped(token.Bool) = %v", x)
	}
	if x, err := vtrue.ToTyped(token.Bool); x != true || err != nil {
		t.Errorf("vtrue.ToTyped(token.Bool) = %v", x)
	}
}

func TestInt(t *testing.T) {
	b1 := big.NewInt(1)
	b2to100 := new(big.Int)
	b2to100.Lsh(b1, 100) // 1<<100 i.e. pow(2, 100)

	v2to7 := Make(Int, int64(128))
	v2to100 := Make(Int, b2to100)

	if x := v2to7.Kind(); x != Int {
		t.Errorf("v2to7.Kind() = %v", x)
	}
	if x := v2to100.Kind(); x != Int {
		t.Errorf("v2to100.Kind() = %v", x)
	}
	if x, exact := v2to7.Int64(); x != int64(128) || !exact {
		t.Errorf("v2to7.Int64() = %v, %v", x, exact)
	}
	if x, err := v2to100.Int64(); err {
		t.Errorf("v2to100.Int64() = %v, %v", x, err)
	}
	if x, err := v2to7.ToTyped(token.Int8); err == nil {
		t.Errorf("v2to7.ToTyped(token.Int8) = %v, %v", x, err)
	}
	if x, err := v2to7.ToTyped(token.Uint8); x != uint64(128) || err != nil {
		t.Errorf("v2to7.ToTyped(token.Uint8) = %v, %v", x, err)
	}
	if x, err := v2to100.ToTyped(token.Int64); err == nil {
		t.Errorf("v2to100.ToTyped(token.Int64) = %v, %v", x, err)
	}
	if x, y := v2to100.String(), "{untyped.Int 1267650600228229401496703205376}"; x != y {
		t.Errorf("v2to100.String() = %s", x)
	}
}

func TestRune(t *testing.T) {
	veur := Make(Rune, int64('\u20ac'))

	if x := veur.Kind(); x != Rune {
		t.Errorf("veur.Kind() = %v", x)
	}
	if x, exact := veur.Int64(); x != int64('\u20ac') || !exact {
		t.Errorf("veur.Int64() = %v, %v", x, exact)
	}
	if x, err := veur.ToTyped(token.Int8); err == nil {
		t.Errorf("veur.ToTyped(token.Int8) = %v, %v", x, err)
	}
	if x, err := veur.ToTyped(token.Uint8); err == nil {
		t.Errorf("veur.ToTyped(token.Uint8) = %v, %v", x, err)
	}
	if x, err := veur.ToTyped(token.Int16); x != int64('\u20ac') || err != nil {
		t.Errorf("veur.ToTyped(token.Int16) = %v, %v", x, err)
	}
	if x, err := veur.ToTyped(token.Int32); x != int64('\u20ac') || err != nil {
		t.Errorf("veur.ToTyped(token.Int32) = %v, %v", x, err)
	}
	if x, y := veur.String(), "{untyped.Rune 8364}"; x != y {
		t.Errorf("veur.String() = %s", x)
	}
}

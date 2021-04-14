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
	if x, exact := vfalse.ToKind(token.Bool); x != false || !exact {
		t.Errorf("vfalse.ToKind(token.Bool) = %v", x)
	}
	if x, exact := vtrue.ToKind(token.Bool); x != true || !exact {
		t.Errorf("vtrue.ToKind(token.Bool) = %v", x)
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
	if x, exact := v2to100.Int64(); exact {
		t.Errorf("v2to100.Int64() = %v, %v", x, exact)
	}
	if x, exact := v2to7.ToKind(token.Int8); exact {
		t.Errorf("v2to7.ToKind(token.Int8) = %v, %v", x, exact)
	}
	if x, exact := v2to7.ToKind(token.Uint8); x != uint64(128) || !exact {
		t.Errorf("v2to7.ToKind(token.Uint8) = %v, %v", x, exact)
	}
	if x, exact := v2to100.ToKind(token.Int64); exact {
		t.Errorf("v2to100.ToKind(token.Int64) = %v, %v", x, exact)
	}
	if x, y := v2to100.String(), "{untyped.Int 1267650600228229401496703205376}"; x != y {
		t.Errorf("v2to100.String() = %x", x)
	}
}

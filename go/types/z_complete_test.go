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
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"testing"
)

func TestCompleteNamed(test *testing.T) {
	/*
	 * runtime equivalent of
	 * type foo bar
	 * type bar *foo
	 */
	foo := NewNamed("foo", "")
	bar := NewNamed("bar", "")
	foo.SetUnderlying(bar)
	bar.SetUnderlying(NewPointer(foo))

	cs := CompleteTypes(foo, bar)
	test.Log(cs)
}

func TestCompleteFunc(test *testing.T) {
	/*
	 * runtime equivalent of
	 * type f func() f
	 */
	f := NewNamed("f", "")
	f.SetUnderlying(NewSignature(nil, []Type{f}, false))

	cs := CompleteTypes(f)
	test.Log(cs)
}

func TestCompleteMap(test *testing.T) {
	/*
	 * runtime equivalent of
	 * type s struct { _ *map[s]bool }
	 */
	s := NewNamed("s", "")
	s.SetUnderlying(NewStruct(Field{
		Name: "_", Type: NewPointer(NewMap(s, BasicType(Bool)))},
	))

	cs := CompleteTypes(s)
	test.Log(cs)
}

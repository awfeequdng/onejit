/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2019-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_test.go
 *
 *  Created on Dec 23, 2020
 *      Author Massimiliano Ghilardi
 */

package common

import (
	"testing"
)

func TestUnary(t *testing.T) {
	Unary(SUB, ConstInt(7))
}

func TestBinary(t *testing.T) {
	var e Expr = Binary(ADD, ConstInt(3), ConstInt(4))
	if !e.IsConst() {
		t.Errorf("expression should be constant")
	}
}

func TestFunc(t *testing.T) {
	/*
	 * emulate the following code:
	 * func fib(n uintptr) uintptr {
	 *   if n > 2 {
	 *     n = fib(n-1) + fib(n-2)
	 *	 } else {
	 *	   n = 1
	 *	 }
	 *   return n
	 * }
	 */
	f := NewFunc("fib", NewSignature([]Kind{Uintptr}, []Kind{Uintptr}))
	small, out := f.NewLabel(), f.NewLabel()
	n := f.Arg(0)
	f.Block(
		Binary(JUMP_IF, small, Binary(LEQ, n, ConstUintptr(2))),
		Binary(ASSIGN, n,
			Binary(ADD,
				f.NewCall(
					f.Label(), f.Signature(),
					[]Expr{
						Binary(SUB, n, ConstUintptr(1)),
					},
				),
				f.NewCall(
					f.Label(), f.Signature(),
					[]Expr{
						Binary(SUB, n, ConstUintptr(2)),
					},
				))),
		Unary(JUMP, out),
		small,
		Binary(ASSIGN, n, ConstUintptr(1)),
		out,
		Tuple(RET, n),
	)

	if f.Signature().NumIn() != 1 || f.Signature().NumOut() != 1 {
		t.Errorf("bad function signature")
	}
	t.Log(f)
}

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

package jit

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
	n := f.Arg(0)
	f.AddStmt(
		If(Binary(GTR, n, ConstUintptr(2)),
			// then
			ToStmt(
				Binary(ASSIGN, n,
					Binary(ADD,
						Call(
							f.Label(), f.Signature(),
							[]Expr{
								Binary(SUB, n, ConstUintptr(1)),
							},
						),
						Call(
							f.Label(), f.Signature(),
							[]Expr{
								Binary(SUB, n, ConstUintptr(2)),
							},
						)))),
			// else
			ToStmt(
				Binary(ASSIGN, n, ConstUintptr(1)))))
	f.AddExpr(Tuple(RET, n))

	if f.Signature().NumIn() != 1 || f.Signature().NumOut() != 1 {
		t.Errorf("bad function signature")
	}
	t.Log(f)
}

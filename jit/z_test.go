/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
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

package jit_test

import (
	"testing"

	. "github.com/cosmos72/gomacrojit/jit"
	_ "github.com/cosmos72/gomacrojit/jit/amd64"
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

func TestFuncRecursion(t *testing.T) {
	/*
	 * compile the following code:
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
	f.Add(
		If(Binary(GTR, n, ConstUintptr(2)),
			// then
			ToStmt(
				Binary(ASSIGN, n,
					Binary(ADD,
						CallFunc(f,
							Binary(SUB, n, ConstUintptr(1)),
						),
						CallFunc(f,
							Binary(SUB, n, ConstUintptr(2)),
						)))),
			// else
			ToStmt(
				Binary(ASSIGN, n, ConstUintptr(1)))),
		Tuple(RET, n))

	if f.Signature().NumIn() != 1 || f.Signature().NumOut() != 1 {
		t.Errorf("bad function signature")
	}
	t.Log("source\n", f)
	f.Compile()
	compiled := f.Compile()
	t.Log("compiled\n", compiled)
	arch_compiled := compiled.ToArch(AMD64)
	t.Log("amd64 compiled\n", arch_compiled)
}

func TestFuncIteration(t *testing.T) {
	/*
	 * compile the following code:
	 * func collatz(n uintptr) {
	 *   for n != 1 {
	 *     if (n & 1) != 0 {
	 *       n = (n * 3) + 1
	 *     }
	 *     n /= 2
	 *   }
	 * }
	 */
	f := NewFunc("collatz", NewSignature([]Kind{Uintptr}, nil))
	n := f.Arg(0)
	one := ConstUintptr(1)
	f.Add(
		For(nil, Binary(NEQ, n, one), nil,
			Block(
				If(Binary(NEQ, Binary(AND, n, one), ConstUintptr(0)),
					ToStmt(Binary(ASSIGN, n, Binary(ADD, Binary(MUL, n, ConstUintptr(3)), one))),
					nil),
				ToStmt(Binary(QUO_ASSIGN, n, ConstUintptr(2))))))
	t.Log("source\n", f)
	f.Compile()
	compiled := f.Compile()
	t.Log("compiled\n", compiled)
	arch_compiled := compiled.ToArch(AMD64)
	t.Log("amd64 compiled\n", arch_compiled)
}

/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * z_test.go
 *
 *  Created on Dec 23, 2020
 *      Author Massimiliano Ghilardi
 */

package jit_test

import (
	"testing"

	. "github.com/cosmos72/onejit/go/jit"
	_ "github.com/cosmos72/onejit/go/jit/amd64"
)

func init() {
	// needed by Compiled.ToArch(AMD64) below
	SetCpuWidth(CpuWidth64)
}

func TestUnary(t *testing.T) {
	e := Unary(SUB, ConstInt(7))
	if !e.IsConst() {
		t.Errorf("expression should be constant")
	}
	t.Log(e)
}

func TestBinary(t *testing.T) {
	var e Expr = Binary(ADD, ConstInt(3), ConstInt(4))
	if !e.IsConst() {
		t.Errorf("expression should be constant")
	}
	t.Log(e)
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
						CallFunc(f, Binary(SUB, n, ConstUintptr(1))),
						CallFunc(f, Binary(SUB, n, ConstUintptr(2)))))),
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
	t.Log("amd64 asm\n", arch_compiled)
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
	t.Log("amd64 asm\n", arch_compiled)
}

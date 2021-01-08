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
 * compile_binary.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/onejit/go/jit/internal"
)

func compileBinary(e *BinaryExpr, toplevel bool, ac *Asm) Expr {
	op := e.Op()
	var ret Expr
	switch op {
	case ADD, SUB, MUL, AND, OR, XOR:
		ret = compileClassicBinary(e, toplevel, ac)

	case QUO, REM, AND_NOT:
		// TODO
		ret = compileClassicBinary(e, toplevel, ac)

	case SHL, SHR:
		// TODO
		ret = compileClassicBinary(e, toplevel, ac)

	case ASSIGN:
		ret = compileAssign(e, ac)

	case ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, QUO_ASSIGN,
		REM_ASSIGN, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN, AND_NOT_ASSIGN,
		SHL_ASSIGN, SHR_ASSIGN:
		// TODO
		ret = compileClassicBinary(e, toplevel, ac)

	case LAND, LOR:
		// TODO
		ret = compileClassicBinary(e, toplevel, ac)

	case EQL, NEQ:
		// TODO
		ret = compileClassicBinary(e, toplevel, ac)

	case LSS, GTR, LEQ, GEQ:
		// TODO
		ret = compileClassicBinary(e, toplevel, ac)

	case JUMP_IF:
		return compileJumpIf(e, ac)

	case RET:
		// TODO

	default:
		BadOpKind2(op, e.X().Kind(), e.Y().Kind())
	}
	if ret == nil {
		Warnf("unimplemented amd64.compileBinary() for %v", e)
		ret = e
	}
	return ret
}

func compileClassicBinary(e *BinaryExpr, toplevel bool, ac *Asm) Expr {
	x, y := compileClassicOperands(e.X(), e.Y(), ac)
	ret := Binary(e.Op(), x, y)
	if !toplevel {
		return SpillToReg(ret, ac)
	}
	return ret
}

// force x,y to one of the combinations natively supported by Amd64 instructions:
// Reg, Reg
// Reg, Mem
// Reg, Const
// Mem, Reg
// Mem, Const
func compileClassicOperands(x Expr, y Expr, ac *Asm) (Expr, Expr) {
	c1, c2 := x.Class(), y.Class()
	switch c1 {
	case REG:
		break
	default:
		x = compileRegOrMem(x, ac)
		c1 = x.Class()
	}
	switch c2 {
	case REG:
		break
	case CONSTANT:
		y = compileConst(y.(Const), ac)
	default:
		if c1 == REG {
			y = compileRegOrMem(y, ac)
		} else {
			y = compileReg(y, ac)
		}
	}
	return x, y
}

func compileAssign(e *BinaryExpr, ac *Asm) Expr {
	op, x, y := e.Op(), e.X(), e.Y()
	var ret Expr
	switch ye := y.(type) {
	case *UnaryExpr:
		switch ye.Op() {
		case NEG, INV, LNOT:
			x = compileRegOrMem(x, ac)
			y = compileUnary(ye, false, ac)
			ret = Binary(op, x, y)
		}
	case *BinaryExpr:
		switch ye.Op() {
		case ADD, SUB, MUL:
			if canCompileAsLea(ye) {
				y = compileAsLea(ye, ac)
				x = compileReg(x, ac)
				ret = Binary(X86_LEA, x, y)
				break
			}
			fallthrough
		case AND, OR, XOR, QUO, REM, AND_NOT,
			LAND, LOR, EQL, NEQ, LSS, GTR, LEQ, GEQ:
			y = compileClassicBinary(ye, true, ac)
			x = compileRegOrMem(x, ac)
			ret = Binary(op, x, y)
		}
	}
	if ret == nil {
		ret = compileClassicBinary(e, true, ac)
	}
	return ret
}

func compileJumpIf(e *BinaryExpr, ac *Asm) Expr {
	op, x, y := e.Op(), e.X(), e.Y()

	KindMustBe(op, x.Kind(), Ptr)
	KindMustBe(op, y.Kind(), Bool)

	l, ok1 := x.(Label)
	cond, ok2 := y.(*BinaryExpr)
	if !ok1 || !ok2 {
		Warnf("unimplemented amd64.compileJumpIf() for %v", e)
		return e
	}
	op = cond.Op()
	if !op.IsComparison() {
		Warnf("unimplemented amd64.compileJumpIf() for %v", e)
		return e
	}
	isTest := canCompileAsTest(cond)
	if isTest {
		op, x, y = compileAsTest(cond, ac)
	} else {
		op, x, y = compileAsCmp(cond, ac)
	}
	ac.Add(Binary(ChooseOp(isTest, X86_TEST, ARCH_CMP), x, y))
	return Unary(ToConditionalJump(op), l)
}

// compile (a cmp b) with cmp being == != < > <= >=
func compileAsCmp(cond *BinaryExpr, ac *Asm) (Op, Expr, Expr) {
	op := cond.Op()
	x := Compile(cond.X(), false, ac)
	y := Compile(cond.Y(), false, ac)
	if x.Class() > y.Class() {
		op = SwapOp(op)
		x, y = y, x
	}
	x, y = compileClassicOperands(x, y, ac)
	return op, x, y
}

// true if expression is ((a AND b) == 0) or ((a AND b) != 0)
func canCompileAsTest(e *BinaryExpr) bool {
	op := e.Op()
	var ret bool
	if (op == EQL || op == NEQ) && IsZero(e.Y()) {
		ab, ok := e.X().(*BinaryExpr)
		ret = (ok && ab.Op() == AND)
	}
	return ret
}

// compile ((a AND b) == 0) or ((a AND b) != 0)
func compileAsTest(e *BinaryExpr, ac *Asm) (Op, Expr, Expr) {
	ab := e.X().(*BinaryExpr)
	x, y := compileClassicOperands(ab.X(), ab.Y(), ac)
	op := e.Op()
	if op == NEQ {
		op = X86_TEST
	} else {
		op = X86_TESTZ
	}
	return op, x, y
}

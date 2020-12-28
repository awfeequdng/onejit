/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * compile_binary.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/gomacrojit/jit/internal"
)

func compileBinary(e *BinaryExpr, toplevel bool, ac *ArchCompiled) Expr {
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
		Warnf("toAmd64Binary: unimplemented %v", e)
		ret = e
	}
	return ret
}

func compileClassicBinary(e *BinaryExpr, toplevel bool, ac *ArchCompiled) Expr {
	x, y := compileClassicOperands(e.X(), e.Y(), ac)
	e = Binary(e.Op(), x, y)
	if !toplevel {
		return SpillToReg(e, ac)
	}
	return e
}

// force x,y to one of the combinations natively supported by Amd64 instructions:
// Reg, Reg
// Reg, Mem
// Reg, Const
// Mem, Reg
// Mem, Const
func compileClassicOperands(x Expr, y Expr, ac *ArchCompiled) (Expr, Expr) {
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

func compileAssign(e *BinaryExpr, ac *ArchCompiled) Expr {
	op, x, y := e.Op(), e.X(), e.Y()
	var ret Expr
	switch ye := y.(type) {
	case *UnaryExpr:
		switch y.Op() {
		case NEG, INV, LNOT:
			x = compileRegOrMem(x, ac)
			y = compileUnary(ye, false, ac)
			ret = Binary(op, x, y)
		}
	case *BinaryExpr:
		switch y.Op() {
		case ADD, SUB, MUL, AND, OR, XOR, QUO, REM, AND_NOT,
			LAND, LOR, EQL, NEQ, LSS, GTR, LEQ, GEQ:
			x = compileRegOrMem(x, ac)
			y = compileClassicBinary(ye, true, ac)
			ret = Binary(op, x, y)
		}
	}
	if ret == nil {
		ret = compileClassicBinary(e, true, ac)
	}
	return ret
}

func compileJumpIf(e *BinaryExpr, ac *ArchCompiled) Expr {
	op, x, y := e.Op(), e.X(), e.Y()

	KindMustBe(op, x.Kind(), Ptr)
	KindMustBe(op, y.Kind(), Bool)

	l, ok1 := x.(Label)
	cond, ok2 := y.(*BinaryExpr)
	if !ok1 || !ok2 {
		Warnf("toAmd64Binary: unimplemented %v", e)
		return e
	}
	op = cond.Op()
	if !op.IsComparison() {
		Warnf("toAmd64Binary: unimplemented %v", e)
		return e
	}
	x = Compile(cond.X(), false, ac)
	y = Compile(cond.Y(), false, ac)

	if x.Class() > y.Class() {
		x, y = y, x
		op = SwapComparison(op)
	}
	x, y = compileClassicOperands(x, y, ac)
	ac.Add(Binary(ARCH_CMP, x, y))
	return Unary(ToConditionalJump(op), l)
}
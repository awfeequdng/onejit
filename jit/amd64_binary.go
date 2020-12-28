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
 * amd64_binary.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

func toAmd64Binary(e *BinaryExpr, toplevel bool, ac *ArchCompiled) Expr {
	op, x, y := e.Op(), e.X(), e.Y()
	k1, k2 := x.Kind(), y.Kind()
	var ret Expr
	switch op {
	case ADD, SUB, MUL, AND, OR, XOR:
		ret = toAmd64ClassicBinary(e, toplevel, ac)

	case QUO, REM, AND_NOT:
		// TODO
		ret = toAmd64ClassicBinary(e, toplevel, ac)

	case SHL, SHR:
		// TODO
		ret = toAmd64ClassicBinary(e, toplevel, ac)

	case ASSIGN:
		switch ye := y.(type) {
		case *UnaryExpr:
			switch ye.Op() {
			case NEG, INV, LNOT:
				x = toAmd64RegOrMem(x, ac)
				y = toAmd64(ye, false, ac)
				ret = Binary(op, x, y)
			}
		case *BinaryExpr:
			switch ye.Op() {
			case ADD, SUB, MUL, AND, OR, XOR, QUO, REM, AND_NOT,
				LAND, LOR, EQL, NEQ, LSS, GTR, LEQ, GEQ:
				x = toAmd64RegOrMem(x, ac)
				y = toAmd64ClassicBinary(ye, true, ac)
				ret = Binary(op, x, y)
			}
		}
		if ret == nil {
			ret = toAmd64ClassicBinary(e, true, ac)
		}

	case ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, QUO_ASSIGN,
		REM_ASSIGN, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN, AND_NOT_ASSIGN,
		SHL_ASSIGN, SHR_ASSIGN:
		// TODO
		ret = toAmd64ClassicBinary(e, toplevel, ac)

	case LAND, LOR:
		// TODO
		ret = toAmd64ClassicBinary(e, toplevel, ac)

	case EQL, NEQ:
		// TODO
		ret = toAmd64ClassicBinary(e, toplevel, ac)

	case LSS, GTR, LEQ, GEQ:
		// TODO
		ret = toAmd64ClassicBinary(e, toplevel, ac)

	case JUMP_IF:
		return toAmd64JumpIf(e, ac)

	case RET:
		// TODO

	default:
		badOpKind2(op, k1, k2)
	}
	if ret == nil {
		Warnf("toAmd64Binary: unimplemented %v", e)
		ret = e
	}
	return ret
}

func toAmd64ClassicBinary(e *BinaryExpr, toplevel bool, ac *ArchCompiled) Expr {
	x, y := toAmd64ClassicOperands(e.X(), e.Y(), ac)
	e = Binary(e.Op(), x, y)
	if !toplevel {
		return spillToReg(e, ac)
	}
	return e
}

func toAmd64JumpIf(e *BinaryExpr, ac *ArchCompiled) Expr {
	op, x, y := e.Op(), e.X(), e.Y()

	x.Kind().mustBePtr(op)
	y.Kind().mustBeBool(op)

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
	x = toAmd64(cond.X(), false, ac)
	y = toAmd64(cond.Y(), false, ac)

	if x.Class() > y.Class() {
		x, y = y, x
		op = swapComparison(op)
	}
	x, y = toAmd64ClassicOperands(x, y, ac)
	ac.Add(Binary(ARCH_CMP, x, y))
	return Unary(toConditionalJump(op), l)
}

// force x,y to one of the combinations natively supported by Amd64 instructions:
// Reg, Reg
// Reg, Mem
// Reg, Const
// Mem, Reg
// Mem, Const
func toAmd64ClassicOperands(x Expr, y Expr, ac *ArchCompiled) (Expr, Expr) {
	c1, c2 := x.Class(), y.Class()
	switch c1 {
	case REG:
		break
	default:
		x = toAmd64RegOrMem(x, ac)
		c1 = x.Class()
	}
	switch c2 {
	case REG:
		break
	case CONSTANT:
		y = toAmd64Const(y.(Const), ac)
	default:
		if c1 == REG {
			y = toAmd64RegOrMem(y, ac)
		} else {
			y = toAmd64Reg(y, ac)
		}
	}
	return x, y
}

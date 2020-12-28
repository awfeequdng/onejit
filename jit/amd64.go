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
 * amd64.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

// convert a generic Expr to a concrete Expr for amd64 architecture.
// may allocate registers for intermediate expressions
func toAmd64(expr Expr, toplevel bool, ac *ArchCompiled) Expr {
	var ret Expr
	switch e := expr.(type) {
	case Const:
		if toplevel { // no side effect, optimize away
			break
		}
		ret = toAmd64Const(e, ac)
	case Label:
		if toplevel {
			ret = expr
		} else {
			ret = e.spillToReg(ac)
		}
	case Reg:
		if toplevel { // no side effect, optimize away
			break
		}
		ret = expr
	case Mem:
		if toplevel { // no side effect, optimize away
			break
		}
		ret = toAmd64Mem(e, ac)
	case *UnaryExpr:
		ret = toAmd64Unary(e, toplevel, ac)
	case *BinaryExpr:
		ret = toAmd64Binary(e, toplevel, ac)
	case *TupleExpr:
		ret = toAmd64Tuple(e, toplevel, ac)
	case *CallExpr:
		ret = toAmd64Call(e, toplevel, ac)
	}
	return ret
}

func toAmd64Const(c Const, ac *ArchCompiled) Expr {
	k := c.Kind()
	if !k.isIntegerOrPtr() {
		Errorf("toAmd64Const: unsupported %v const", k)
	}
	if _, ok := toInt32(c); ok {
		return c
	}
	// constant value is too large, save it in a register
	return c.spillToReg(ac)
}

// convert a generic Expr to a concrete Expr for amd64 architecture,
// and copy its result to a temporary register.
// may allocate registers for intermediate expressions
func toAmd64Reg(e Expr, ac *ArchCompiled) Expr {
	if e.Class() != REG {
		e = toAmd64(e, false, ac)
		e = spillToReg(e, ac)
	}
	return e
}

// convert a generic Expr to a concrete Expr for amd64 architecture,
// and return its result either as a temporary register or as a memory reference.
// may allocate registers for intermediate expressions
func toAmd64RegOrMem(e Expr, ac *ArchCompiled) Expr {
	switch e.Class() {
	case REG:
		break
	case MEM:
		e = toAmd64Mem(e.(Mem), ac)
	default:
		e = toAmd64Reg(e, ac)
	}
	return e
}

// convert a generic Expr to a concrete Expr for amd64 architecture,
// and return its result either as a temporary register or as a constant.
// may allocate registers for intermediate expressions
func toAmd64RegOrConst(e Expr, ac *ArchCompiled) Expr {
	switch e.Class() {
	case REG:
		break
	case CONSTANT:
		e = toAmd64Const(e.(Const), ac)
	default:
		e = toAmd64Reg(e, ac)
	}
	return e
}

func toAmd64Unary(e *UnaryExpr, toplevel bool, ac *ArchCompiled) Expr {
	op, x := e.Op(), e.X()
	k := x.Kind()
	switch op {

	case NEG, INV: // unary - !
		if toplevel { // no side effect, optimize away
			return nil
		}
		k.mustBeNumberOrPtr(op)

	case INC, DEC:
		k.mustBeNumberOrPtr(op)
		mustBeAssignable(op, x)

	case LNOT: // unary !
		k.mustBeBool(op)
		if toplevel { // no side effect, optimize away
			return nil
		}
		return Binary(XOR, toAmd64(x, false, ac), MakeConst(k, 1))

	case JUMP:
		k.mustBePtr(op)
		if toplevel && x.Class() == LABEL {
			return e
		}

	case ZERO: // clear (i.e set to zero) register or memory
		mustBeAssignable(op, x)

	case RET:

	default:
		badOpKind(op, k)
	}
	return Unary(op, toAmd64(x, false, ac))
}

func toAmd64Tuple(e *TupleExpr, toplevel bool, ac *ArchCompiled) Expr {
	Warnf("toAmd64Tuple: unimplemented %v", e)
	return e
}

func toAmd64Call(e *CallExpr, toplevel bool, ac *ArchCompiled) Expr {
	Warnf("toAmd64Call: unimplemented %v", e)
	return e
}

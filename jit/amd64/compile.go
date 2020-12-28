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
 * compile.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/gomacrojit/jit/internal"
)

func init() {
	Archs[AMD64] = Arch{
		AMD64,
		Compile,
	}
}

// convert a generic Expr to a concrete Expr for amd64 architecture.
// may allocate registers for intermediate expressions
func Compile(expr Expr, toplevel bool, ac *ArchCompiled) Expr {
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
			ret = SpillToReg(expr, ac)
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
	if !k.IsIntegerOrPtr() {
		Errorf("toAmd64Const: unsupported %v const", k)
	}
	if _, ok := toInt32(c); ok {
		return c
	}
	// constant value is too large, save it in a register
	return SpillToReg(c, ac)
}

// convert a generic Expr to a concrete Expr for amd64 architecture,
// and copy its result to a temporary register.
// may allocate registers for intermediate expressions
func toAmd64Reg(e Expr, ac *ArchCompiled) Expr {
	if e.Class() != REG {
		e = Compile(e, false, ac)
		e = SpillToReg(e, ac)
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
		KindMustBeNumberOrPtr(op, k)

	case INC, DEC:
		KindMustBeNumberOrPtr(op, k)
		ExprMustBeAssignable(op, x)

	case LNOT: // unary !
		KindMustBe(op, k, Bool)
		if toplevel { // no side effect, optimize away
			return nil
		}
		return Binary(XOR, Compile(x, false, ac), MakeConst(k, 1))

	case JUMP:
		KindMustBe(op, k, Ptr)
		if toplevel && x.Class() == LABEL {
			return e
		}

	case ZERO: // clear (i.e set to zero) register or memory
		ExprMustBeAssignable(op, x)

	case RET:

	default:
		BadOpKind(op, k)
	}
	return Unary(op, Compile(x, false, ac))
}

func toAmd64Tuple(e *TupleExpr, toplevel bool, ac *ArchCompiled) Expr {
	Warnf("toAmd64Tuple: unimplemented %v", e)
	return e
}

func toAmd64Call(e *CallExpr, toplevel bool, ac *ArchCompiled) Expr {
	Warnf("toAmd64Call: unimplemented %v", e)
	return e
}

func toInt32(c Const) (int32, bool) {
	KindMustBeIntegerOrPtr(STAR, c.Kind())
	val := c.Int()
	if val != int64(int32(val)) {
		return 0, false
	}
	return int32(val), true
}

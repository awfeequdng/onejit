/*
 * onejit - JIT compiler in Go
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
	. "github.com/cosmos72/onejit/jit/internal"
)

func init() {
	Archs[AMD64] = Arch{
		AMD64,
		CpuWidth64,
		Compile,
	}
}

// convert a generic Expr to a concrete Expr for amd64 architecture.
// may allocate registers for intermediate expressions
func Compile(expr Expr, toplevel bool, ac *Asm) Expr {
	var ret Expr
	if toplevel && expr.IsPure() {
		Debugf("expression has no effect: %v", expr)
		return ret
	}
	switch e := expr.(type) {
	case Const:
		ret = compileConst(e, ac)
	case Label:
		if toplevel {
			ret = expr
		} else {
			ret = SpillToReg(expr, ac)
		}
	case Reg:
		ret = expr
	case Mem:
		ret = compileMem(e, ac)
	case *UnaryExpr:
		ret = compileUnary(e, toplevel, ac)
	case *BinaryExpr:
		ret = compileBinary(e, toplevel, ac)
	case *TupleExpr:
		ret = compileTuple(e, toplevel, ac)
	case *CallExpr:
		ret = compileCall(e, toplevel, ac)
	}
	return ret
}

func compileConst(c Const, ac *Asm) Expr {
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
func compileReg(e Expr, ac *Asm) Reg {
	var ret Reg
	if e.Class() == REG {
		ret = e.(Reg)
	} else {
		e = Compile(e, false, ac)
		ret = SpillToReg(e, ac)
	}
	return ret
}

// convert a generic Expr to a concrete Expr for amd64 architecture,
// and return its result either as a temporary register or as a memory reference.
// may allocate registers for intermediate expressions
func compileRegOrMem(e Expr, ac *Asm) Expr {
	switch e.Class() {
	case REG:
		break
	case MEM:
		e = compileMem(e.(Mem), ac)
	default:
		e = compileReg(e, ac)
	}
	return e
}

// convert a generic Expr to a concrete Expr for amd64 architecture,
// and return its result either as a temporary register or as a constant.
// may allocate registers for intermediate expressions
func compileRegOrConst(e Expr, ac *Asm) Expr {
	switch e.Class() {
	case REG:
		break
	case CONSTANT:
		e = compileConst(e.(Const), ac)
	default:
		e = compileReg(e, ac)
	}
	return e
}

func compileUnary(e *UnaryExpr, toplevel bool, ac *Asm) Expr {
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

func compileTuple(e *TupleExpr, toplevel bool, ac *Asm) Expr {
	Warnf("unimplemented amd64.compileTuple() %v", e)
	return e
}

func compileCall(e *CallExpr, toplevel bool, ac *Asm) Expr {
	Warnf("unimplemented amd64.compileCall() %v", e)
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

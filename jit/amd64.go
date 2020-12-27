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
	e = toAmd64(e, false, ac)
	if e.Class() != REG {
		reg := ac.Func().NewReg(e.Kind())
		ac.Add(Binary(ASSIGN, reg, e))
		e = reg
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

func toAmd64Binary(e *BinaryExpr, toplevel bool, ac *ArchCompiled) Expr {
	op, x, y := e.Op(), e.X(), e.Y()
	k1, k2 := x.Kind(), y.Kind()
	var ret Expr
	switch op {
	case ADD, SUB, MUL, QUO:
		// TODO

	case REM, AND, OR, XOR, AND_NOT:
		// TODO

	case SHL, SHR:
		// TODO

	case ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, QUO_ASSIGN:
		// TODO

	case REM_ASSIGN, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN, AND_NOT_ASSIGN:
		// TODO

	case ASSIGN:
		// TODO

	case SHL_ASSIGN, SHR_ASSIGN:
		// TODO

	case LAND, LOR:
		// TODO

	case EQL, NEQ:
		// TODO

	case LSS, GTR, LEQ, GEQ:
		// TODO

	case JUMP_IF:
		k1.mustBePtr(op)
		k2.mustBeBool(op)

		l, ok1 := x.(Label)
		cond, ok2 := y.(*BinaryExpr)
		if !ok1 || !ok2 || !cond.Op().IsComparison() {
			Errorf("toAmd64Binary: unimplemented %v", e)
		}
		op := cond.Op()
		x = toAmd64(cond.X(), false, ac)
		y = toAmd64(cond.Y(), false, ac)

		if x.Class() > y.Class() {
			x, y = y, x
			op = swapComparison(op)
		}
		x, y := toAmd64ClassicOperands(x, y, ac)
		ac.Add(Binary(cmp, x, y))
		return Unary(toConditionalJump(op), l)

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

func toAmd64Tuple(e *TupleExpr, toplevel bool, ac *ArchCompiled) Expr {
	Warnf("toAmd64Tuple: unimplemented %v", e)
	return e
}

func toAmd64Call(e *CallExpr, toplevel bool, ac *ArchCompiled) Expr {
	Warnf("toAmd64Call: unimplemented %v", e)
	return e
}

func toAmd64Mem(m Mem, ac *ArchCompiled) Expr {
	var ret Amd64Mem
	// kind := m.Kind()
	switch addr := m.Addr().(type) {
	case Const:
		if offset, ok := toInt32(addr); ok {
			ret = MakeAmd64Mem(m.Kind(), offset, NoRegId, NoRegId, 0)
		} else {
			reg := addr.spillToReg(ac)
			ret = MakeAmd64Mem(m.Kind(), 0, reg.RegId(), NoRegId, 0)
		}
	case Reg:
		addr.Kind().mustBeUintptrOrPtr("Amd64Mem")
		ret = MakeAmd64Mem(m.Kind(), 0, addr.RegId(), NoRegId, 0)
	case *BinaryExpr:
		switch addr.Op() {
		case ADD:
			// return addToAmd64Mem(f, expr.X(), expr.Y())
		case SUB:
			// return addToAmd64Mem(f, expr.X(), expr.Y())
		}
		Warnf("toAmd64Mem: unimplemented address type: %T", addr)
		return m
	default:
		Warnf("toAmd64Mem: unsupported address type: %T", addr)
		return m
	}
	return ret
}

func toInt32(c Const) (int32, bool) {
	c.Kind().mustBeIntegerOrPtr(STAR)
	val := c.Int()
	if val != int64(int32(val)) {
		return 0, false
	}
	return int32(val), true
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
	case REG, MEM:
		break
	default:
		x = toAmd64Reg(x, ac)
		c1 = x.Class()
	}
	switch c2 {
	case REG, CONST:
		break
	default:
		if c1 == REG && c2 == MEM {
			break
		}
		y = toAmd64Reg(y, ac)
	}
	return x, y
}

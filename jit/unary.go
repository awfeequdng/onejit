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
 * unaryexpr.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

type UnaryExpr struct {
	_    noteq
	kind Kind
	op   Op
	x    Expr
}

func Unary(op Op, x Expr) Expr {
	kind := unaryKind(op, x)
	if op == LNOT && kind == Bool && x.Class() == BINARY {
		bx := x.(*BinaryExpr)
		bop := bx.Op()
		if bop.IsComparison() {
			// simplify !(a < b) to (a >= b)
			// and similarly for other comparisons
			return Binary(swapComparison(bop), bx.X(), bx.Y())
		}
	}
	return &UnaryExpr{
		kind: kind,
		op:   op,
		x:    x,
	}
}

func Cast(to Kind, x Expr) *UnaryExpr {
	return &UnaryExpr{
		kind: to,
		op:   CAST,
		x:    x,
	}
}

func (e *UnaryExpr) Op() Op {
	return e.op
}

func (e *UnaryExpr) X() Expr {
	return e.x
}

// implement Expr interface
func (e *UnaryExpr) expr() {}

func (e *UnaryExpr) RegId() RegId {
	return NoRegId
}

func (e *UnaryExpr) Kind() Kind {
	return e.kind
}

func (e *UnaryExpr) Size() Size {
	return e.kind.Size()
}

func (e *UnaryExpr) Class() Class {
	return UNARY
}

func (e *UnaryExpr) Children() int {
	return 1
}

func (e *UnaryExpr) Child(i int) Node {
	if i == 0 {
		return e.x
	}
	return badIndex(i, 1)
}

func (e *UnaryExpr) IsConst() bool {
	// address dereference cannot be a constant
	return e.op != MUL && e.x.IsConst()
}

func (e *UnaryExpr) IsPure() bool {
	var ret bool
	switch e.Op() {
	case NEG, INV, LNOT:
		ret = e.x.IsPure()
	}
	return ret
}

// ========================= helpers ===========================================

func unaryKind(op Op, x Expr) Kind {
	k := x.Kind()
	switch op {
	case NEG, INV: // unary - ^
		return k.mustBeNumberOrPtr(op)
	case INC, DEC:
		k.mustBeNumberOrPtr(op)
		return mustBeAssignable(op, x)
	case LNOT: // unary !
		k.mustBeBool(op)
		return Bool
	case JUMP, jeq, jlt, jgt, jne, jle, jge:
		k.mustBePtr(op)
		return Void
	case ZERO: // clear (i.e set to zero) register or memory
		return mustBeAssignable(op, x)
	case RET:
		// RET accepts any kind and returns Void.
		// kind will be checked against function signature
		return Void
	}
	return badOpKind(op, k)
}

func mustBeAssignable(op Op, x Expr) Kind {
	switch x := x.(type) {
	case Reg:
		if x.IsAssignable() {
			return Void
		}
	case Mem:
		if x.IsAssignable() {
			return Void
		}
	}
	Errorf("cannot assign to expression: %v %T", op, x)
	return Void
}

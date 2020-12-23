/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2019-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * unary.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package common

type UnaryExpr struct {
	_    noteq
	kind Kind
	op   Op
	x    Expr
}

func Unary(op Op, x Expr) UnaryExpr {
	return UnaryExpr{
		kind: unaryKind(op, x),
		op:   op,
		x:    x,
	}
}

func Cast(to Kind, x Expr) UnaryExpr {
	return UnaryExpr{
		kind: to,
		op:   CAST,
		x:    x,
	}
}

func (e UnaryExpr) Op() Op {
	return e.op
}

func (e UnaryExpr) X() Expr {
	return e.x
}

// implement Expr interface
func (e UnaryExpr) expr() {}

func (e UnaryExpr) RegId() RegId {
	return NoRegId
}

func (e UnaryExpr) Kind() Kind {
	return e.kind
}

func (e UnaryExpr) Const() bool {
	// address dereference cannot be a constant
	return e.op != MUL && e.x.Const()
}

func (e UnaryExpr) Size() Size {
	return e.kind.Size()
}

// ========================= helpers ===========================================

func unaryKind(op Op, x Expr) Kind {
	k := x.Kind()
	switch op {
	case INC, DEC:
		if k != Ptr {
			k.mustBeNumber(op)
		}
		return nonConst(op, x)
	case NOT:
		k.mustBeBool(op)
		return Bool
	}
	return badOpKind(op, k)
}

func nonConst(op Op, x Expr) Kind {
	if x.Const() {
		Errorf("cannot assign to constant: %v %v", op, x.Kind())
	}
	return Void
}

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
 * tupleexpr.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package common

type TupleExpr struct {
	op   Op
	list []Expr
}

func Tuple(op Op, expr ...Expr) TupleExpr {
	return TupleExpr{
		op:   tupleOp(op),
		list: expr,
	}
}

func TupleSlice(op Op, expr []Expr) TupleExpr {
	return TupleExpr{
		op:   tupleOp(op),
		list: append([]Expr(nil), expr...),
	}
}

func (e TupleExpr) Op() Op {
	return e.op
}

func (e TupleExpr) Len() int {
	return len(e.list)
}

func (e TupleExpr) At(i int) Expr {
	return e.list[i]
}

// implement Expr interface
func (e TupleExpr) expr() {}

func (e TupleExpr) RegId() RegId {
	return NoRegId
}

func (e TupleExpr) Kind() Kind {
	return Void
}

func (e TupleExpr) IsConst() bool {
	return false
}

func (e TupleExpr) Size() Size {
	return Void.Size()
}

// ========================= helpers ===========================================

func tupleOp(op Op) Op {
	switch op {
	case RET: // return
		return op
	}
	badOpKind(op, Void)
	return BADOP
}

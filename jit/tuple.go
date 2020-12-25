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

package jit

type TupleExpr struct {
	op   Op
	list []Expr
}

func Tuple(op Op, expr ...Expr) *TupleExpr {
	return tuple(op, expr)
}

func TupleSlice(op Op, expr []Expr) *TupleExpr {
	return tuple(op, dup(expr))
}

func Ret(expr ...Expr) *TupleExpr {
	return tuple(RET, expr)
}

func RetSlice(expr []Expr) *TupleExpr {
	return tuple(RET, dup(expr))
}

func AssignCall(to []Expr, call *CallExpr) *TupleExpr {
	return tuple(ASSIGN, append(dup(to), call))
}

func (e *TupleExpr) Op() Op {
	return e.op
}

// implement Expr interface
func (e *TupleExpr) expr() {}

func (e *TupleExpr) RegId() RegId {
	return NoRegId
}

func (e *TupleExpr) Kind() Kind {
	return Void
}

func (e *TupleExpr) IsConst() bool {
	return false
}

func (e *TupleExpr) Size() Size {
	return Void.Size()
}

func (e *TupleExpr) Children() int {
	return len(e.list)
}

func (e *TupleExpr) Child(i int) Node {
	return e.list[i]
}

// ========================= helpers ===========================================

func dup(expr []Expr) []Expr {
	ret := make([]Expr, len(expr))
	copy(ret, expr)
	return ret
}

func tuple(op Op, expr []Expr) *TupleExpr {
	switch op {
	case ASSIGN:
		n := len(expr) - 1
		if n < 0 {
			Errorf("missing CallExpr in tuple %v", op)
		}
		call, ok := expr[n].(*CallExpr)
		if !ok {
			Errorf("bad last expression in tuple %v: have %T, want CallExpr", op, expr[n])
		}
		sig := call.Signature()
		if n != sig.NumOut() {
			Errorf("bad number of destinations in tuple %v: have %d, want %d", op, n, sig.NumOut())
		}
		for i := 0; i < n; i++ {
			kind := expr[i].Kind()
			karg := sig.Out(i)
			if karg != kind {
				Errorf("bad destination %v in tuple %v: have %d, want %d", i+1, op, karg, kind)
			}
		}
		break
	case RET:
		break
	default:
		Errorf("invalid tuple operation: %v", op)
	}
	return &TupleExpr{
		op:   op,
		list: expr,
	}
}

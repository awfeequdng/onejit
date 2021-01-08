/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * tuple.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

type TupleExpr struct {
	op   Op
	list []Expr
}

func Tuple(op Op, expr ...Expr) *TupleExpr {
	return tuple(op, dup(expr))
}

func Ret(expr ...Expr) *TupleExpr {
	return tuple(RET, dup(expr))
}

func AssignCall(to []Expr, call *CallExpr) *TupleExpr {
	return tuple(ASSIGN, append(dup(to), call))
}

// implement Expr interface
func (e *TupleExpr) Class() Class {
	return TUPLE
}

func (e *TupleExpr) Op() Op {
	return e.op
}

func (e *TupleExpr) Kind() Kind {
	return Void
}

func (e *TupleExpr) expr() {}

func (e *TupleExpr) RegId() RegId {
	return NoRegId
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

func (e *TupleExpr) IsConst() bool {
	return false
}

func (e *TupleExpr) IsPure() bool {
	// tuples are either assignments or returns => they have side effects
	return false
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

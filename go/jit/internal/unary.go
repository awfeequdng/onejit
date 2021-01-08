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
 * unary.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

type UnaryExpr struct {
	_    noteq
	kind Kind
	op   Op
	x    Expr
}

func Unary(op Op, x Expr) Expr {
	kind := unaryKind(op, x)
	ret := optimizeUnary(op, kind, x)
	if ret == nil {
		ret = &UnaryExpr{
			kind: kind,
			op:   op,
			x:    x,
		}
	}
	return EvalConst(ret)
}

func Cast(to Kind, x Expr) Expr {
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
	return BadIndex(i, 1)
}

func (e *UnaryExpr) IsConst() bool {
	var ret bool
	switch e.Op() {
	// only -x ^x !x can be constant
	case NEG, INV, LNOT:
		ret = e.x.IsConst()
	}
	return ret
}

func (e *UnaryExpr) IsPure() bool {
	var ret bool
	switch e.Op() {
	// only -x ^x !x can be pure
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
		return KindMustBeNumberOrPtr(op, k)
	case INC, DEC:
		KindMustBeNumberOrPtr(op, k)
		return ExprMustBeAssignable(op, x)
	case LNOT: // unary !
		k.mustBe(Bool, op)
		return Bool
	case JUMP, ARCH_JEQ, ARCH_JLT, ARCH_JGT, ARCH_JNE, ARCH_JLE, ARCH_JGE, ARCH_JZ, ARCH_JNZ:
		k.mustBe(Ptr, op)
		return Void
	case ZERO: // clear (i.e set to zero) register or memory
		return ExprMustBeAssignable(op, x)
	case RET:
		// RET accepts any kind and returns Void.
		// kind will be checked against function signature
		return Void
	}
	return BadOpKind(op, k)
}

func optimizeUnary(op Op, kind Kind, x Expr) Expr {
	var ret Expr
	xop := x.Op()
	if op == LNOT && kind == Bool {
		switch x := x.(type) {
		case *UnaryExpr:
			if xop == LNOT {
				// simplify !(!a) to a
				ret = x.X()
			}
		case *BinaryExpr:
			if xop.IsComparison() {
				// simplify !(a < b) to (a >= b)
				// and similarly for other comparisons
				ret = Binary(SwapOp(xop), x.X(), x.Y())
			}
		}
	} else if (op == NEG || op == INV) && (xop == NEG || xop == INV) {
		switch x := x.(type) {
		case *UnaryExpr:
			if op == xop {
				// simplify -(-a) to a
				// simplify ^(^a) to a
				ret = x.X()
			} else if op == NEG {
				// simplify -(^a) to a+1
				ret = Binary(ADD, x.X(), MakeConst(kind, 1))
			} else {
				// simplify ^(-a) to a-1
				ret = Binary(ADD, x.X(), MakeConst(kind, -1))
			}
		}
	}
	return ret
}

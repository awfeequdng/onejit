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
 * binary.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

type BinaryExpr struct {
	_    noteq
	kind Kind
	op   Op
	x    Expr
	y    Expr
}

func Binary(op Op, x Expr, y Expr) *BinaryExpr {
	if op.IsCommutative() && x.Class() > y.Class() {
		x, y = y, x
	}
	return &BinaryExpr{
		kind: binaryKind(op, x, y),
		op:   op,
		x:    x,
		y:    y,
	}
}

func (e *BinaryExpr) Op() Op {
	return e.op
}

func (e *BinaryExpr) X() Expr {
	return e.x
}

func (e *BinaryExpr) Y() Expr {
	return e.y
}

// implement Expr interface
func (e *BinaryExpr) expr() {}

func (e *BinaryExpr) RegId() RegId {
	return NoRegId
}

func (e *BinaryExpr) Kind() Kind {
	return e.kind
}

func (e *BinaryExpr) Size() Size {
	return e.kind.Size()
}

func (e *BinaryExpr) Class() Class {
	return BINARY
}

func (e *BinaryExpr) Children() int {
	return 2
}

func (e *BinaryExpr) Child(i int) Node {
	switch i {
	case 0:
		return e.x
	case 1:
		return e.y
	default:
		return BadIndex(i, 2)
	}
}

func (e *BinaryExpr) IsConst() bool {
	// access to array element or struct field cannot be a constant
	return e.op != BRACKET && e.op != FIELD && e.x.IsConst() && e.y.IsConst()
}

func (e *BinaryExpr) IsPure() bool {
	var ret bool
	switch e.Op() {
	case ADD, SUB, MUL, QUO, REM,
		AND, OR, XOR, AND_NOT,
		SHL, SHR, LAND, LOR,
		EQL, NEQ, LSS, GTR, LEQ, GEQ:

		ret = e.x.IsPure() && e.y.IsPure()
	}
	return ret
}

// ========================= helpers ===========================================

func binaryKind(op Op, x Expr, y Expr) Kind {
	k1, k2 := x.Kind(), y.Kind()
	switch op {
	case ADD, SUB, MUL, QUO:
		return Kind2MustBeNumberOrPtr(op, k1, k2)

	case REM, AND, OR, XOR, AND_NOT:
		return Kind2MustBeIntegerOrPtr(op, k1, k2)

	case SHL, SHR:
		k1.mustBeIntegerOrPtr(op)
		k2.mustBeIntegerOrPtr(op)
		return k1

	case ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, QUO_ASSIGN:
		ExprMustBeAssignable(op, x)
		Kind2MustBeNumberOrPtr(op, k1, k2)
		return Void

	case REM_ASSIGN, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN, AND_NOT_ASSIGN:
		Kind2MustBeIntegerOrPtr(op, k1, k2)
		fallthrough

	case ASSIGN:
		ExprMustBeAssignable(op, x)
		return Void

	case SHL_ASSIGN, SHR_ASSIGN:
		ExprMustBeAssignable(op, x)
		k1.mustBeIntegerOrPtr(op)
		k2.mustBeIntegerOrPtr(op)
		return Void

	case LAND, LOR:
		k1.mustBe(Bool, op)
		k2.mustBe(Bool, op)
		return Bool

	case EQL, NEQ:
		Kind2MustBeComparable(op, k1, k2)
		return Bool

	case LSS, GTR, LEQ, GEQ:
		Kind2MustBeOrdered(op, k1, k2)
		return Bool

	case JUMP_IF:
		k1.mustBe(Ptr, op)
		k2.mustBe(Bool, op)
		return Void

	case RET:
		// RET accepts any kind and returns Void.
		// kinds will be checked against function signature
		return Void

	case ARCH_CMP: // compare, then set arch-specific flags
		Kind2MustBeOrdered(op, k1, k2)
		return ArchFlags

	case X86_TEST: // bitwise-and, then set arch-specific flags
		Kind2MustBeIntegerOrPtr(op, k1, k2)
		return ArchFlags

	default:
		return BadOpKind2(op, k1, k2)
	}
}

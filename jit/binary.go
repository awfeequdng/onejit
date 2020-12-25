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
 * binaryexpr.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

type BinaryExpr struct {
	_    noteq
	kind Kind
	op   Op
	x    Expr
	y    Expr
}

func Binary(op Op, x Expr, y Expr) *BinaryExpr {
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

func (e *BinaryExpr) IsConst() bool {
	// access to array element or struct field cannot be a constant
	return e.op != BRACKET && e.op != FIELD && e.x.IsConst() && e.y.IsConst()
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
		return badIndex(i, 2)
	}
}

// ========================= helpers ===========================================

func binaryKind(op Op, x Expr, y Expr) Kind {
	k1, k2 := x.Kind(), y.Kind()
	switch op {
	case ADD, SUB, MUL, QUO:
		return kindMustBeNumberOrPtr(op, k1, k2)

	case REM, AND, OR, XOR, AND_NOT:
		return kindMustBeIntegerOrPtr(op, k1, k2)

	case SHL, SHR:
		k1.mustBeIntegerOrPtr(op)
		k2.mustBeIntegerOrPtr(op)
		return k1

	case ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, QUO_ASSIGN:
		mustBeAssignable(op, x)
		kindMustBeNumberOrPtr(op, k1, k2)
		return Void

	case REM_ASSIGN, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN, AND_NOT_ASSIGN:
		kindMustBeIntegerOrPtr(op, k1, k2)
		fallthrough

	case ASSIGN:
		mustBeAssignable(op, x)
		return Void

	case SHL_ASSIGN, SHR_ASSIGN:
		mustBeAssignable(op, x)
		k1.mustBeIntegerOrPtr(op)
		k2.mustBeIntegerOrPtr(op)
		return Void

	case LAND, LOR:
		k1.mustBeBool(op)
		k2.mustBeBool(op)
		return Bool

	case EQL, NEQ:
		kindMustBeComparable(op, k1, k2)
		return Bool

	case LSS, GTR, LEQ, GEQ:
		kindMustBeOrdered(op, k1, k2)
		return Bool

	case JUMP_IF:
		k1.mustBePtr(op)
		k2.mustBeBool(op)
		return Void

	case RET:
		// RET accepts any kind and returns Void.
		// kinds will be checked against function signature
		return Void

	default:
		return badOpKind2(op, k1, k2)
	}
}

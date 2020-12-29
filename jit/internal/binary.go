/*
 * onejit - JIT compiler in Go
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

func Binary(op Op, x Expr, y Expr) Expr {
	kind := binaryKind(op, x, y)
	if CanSwapOp(op) && x.Class() > y.Class() {
		op = SwapOp(op)
		x, y = y, x
	}
	ret := optimizeBinary(op, kind, x, y)
	if ret == nil {
		ret = &BinaryExpr{
			kind: kind,
			op:   op,
			x:    x,
			y:    y,
		}
	}
	return EvalConst(ret)
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

	case X86_TEST, X86_TESTZ: // bitwise-and, then set arch-specific flags
		Kind2MustBeIntegerOrPtr(op, k1, k2)
		return ArchFlags

	case X86_LEA: // assign memory address
		Kind2MustBeIntegerOrPtr(op, k1, k2)
		return Void

	default:
		return BadOpKind2(op, k1, k2)
	}
}

func optimizeBinary(op Op, kind Kind, x Expr, y Expr) Expr {
	var ret Expr
	switch op {
	case ADD:
		if IsZero(y) {
			// simplify (x + 0) to x
			ret = x
		}
	case SUB:
		if IsZero(y) {
			// simplify (x - 0) to x
			ret = x
		} else if IsZero(x) {
			// simplify (0 - y) to -y
			ret = Unary(NEG, y)
		} else if y.Kind().IsIntegerOrPtr() {
			c, ok := x.(Const)
			if ok && IsMinusOne(c) {
				// simplify (-1 - y) to ^y
				ret = Unary(INV, y)
			}
		}
	case MUL:
		c, ok := y.(Const)
		if !ok {
			break
		} else if c.IsZero() {
			if x.IsPure() {
				// simplify (x * 0) to 0
				ret = y
			}
		} else if IsOne(c) {
			// simplify (x * 1) to x
			ret = x
		} else if IsMinusOne(c) {
			// simplify (x * -1) to -x
			ret = Unary(NEG, x)
		} else if val, ok := IsIntPowerOf2(c); ok {
			// simplify (x * (1<<k)) to x << k
			k := uintLog2(val)
			ret = Binary(SHL, x, ConstUint8(k))
		}
	case MUL_ASSIGN:
		c, ok := y.(Const)
		if !ok {
			break
		} else if c.IsZero() {
			if x.IsPure() {
				// simplify (x *= 0) to x = 0
				ret = Unary(ZERO, x)
			}
		} else if IsOne(c) {
			// simplify (x *= 1) to x or void
			if x.IsPure() {
				ret = MakeConst(Void, 0)
			} else {
				ret = x
			}
		} else if IsMinusOne(c) {
			// simplify (x *= -1) to x = -x
			ret = Unary(NEG_ASSIGN, x)
		} else if val, ok := IsIntPowerOf2(c); ok {
			// simplify (x *= (1<<k)) to x <<= k
			k := uintLog2(val)
			ret = Binary(SHL_ASSIGN, x, ConstUint8(k))
		}
	case QUO:
		c, ok := y.(Const)
		if !ok {
			break
		} else if IsOne(c) {
			// simplify (x / 1) to x
			ret = x
		} else if IsMinusOne(c) {
			// simplify (x / -1) to -x
			ret = Unary(NEG, x)
		} else if val, ok := IsIntPowerOf2(c); ok && x.Kind().IsUnsigned() {
			// simplify (x / (1<<k)) to x >> k
			k := uintLog2(val)
			ret = Binary(SHR, x, ConstUint8(k))
		}
	case QUO_ASSIGN:
		c, ok := y.(Const)
		if !ok {
			break
		} else if IsOne(c) {
			// simplify (x /= 1) to x or void
			if x.IsPure() {
				ret = MakeConst(Void, 0)
			} else {
				ret = x
			}
		} else if IsMinusOne(c) {
			// simplify (x /= -1) to x = -x
			ret = Unary(NEG_ASSIGN, x)
		} else if val, ok := IsIntPowerOf2(c); ok && x.Kind().IsUnsigned() {
			// simplify (x /= (1<<k)) to x >>= k
			k := uintLog2(val)
			ret = Binary(SHR_ASSIGN, x, ConstUint8(k))
		}
	}
	return ret
}

// return integer log2(val)
func uintLog2(val uint64) uint8 {
	var ret uint8
	for val > 1 {
		ret++
		val >>= 1
	}
	return ret
}

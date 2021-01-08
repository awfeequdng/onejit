/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
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
 * eval.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

// if expression is a constant, return its value
func EvalConst(e Expr) Expr {
	if e.Class() != LABEL && e.IsConst() {
		e = evalConst(e)
	}
	return e
}

func badConst(e Expr) {
	Errorf("EvalConst: expression is not a constant: %v", e)
}

func evalConst(e Expr) Const {
	var c Const
	switch e := e.(type) {
	case Const:
		c = e
	case *UnaryExpr:
		c = evalUnary(e)
	case *BinaryExpr:
		c = evalBinary(e)
	default:
		badConst(e)
	}
	return c
}

func evalUnary(e *UnaryExpr) Const {
	c := evalConst(e.X())
	switch e.Op() {
	case NEG:
		if c.kind == Float32 {
			c = ConstFloat32(-c.Float32())
		} else if c.kind == Complex64 {
			c = ConstComplex64(-c.Complex64())
		} else {
			c.re = -c.re
			c.im = -c.im
		}
	case INV:
		c.re = ^c.re
		c.im = ^c.im
	case LNOT:
		c.re ^= 1
	default:
		badConst(e)
	}
	return c
}

func evalBinary(e *BinaryExpr) Const {
	x, y := evalConst(e.X()), evalConst(e.Y())
	fn := evalBinaryFunc[e.Op()]
	if fn == nil {
		badConst(e)
	}
	return fn(x, y)
}

var evalBinaryFunc = map[Op]func(Const, Const) Const{
	ADD:     addConst,
	SUB:     subConst,
	MUL:     mulConst,
	QUO:     quoConst,
	REM:     remConst,
	AND:     andConst,
	OR:      orConst,
	XOR:     xorConst,
	SHL:     shlConst,
	SHR:     shrConst,
	AND_NOT: andNotConst,
	LAND:    landConst,
	LOR:     lorConst,
	EQL:     eqlConst,
	LSS:     lssConst,
	GTR:     gtrConst,
	NEQ:     neqConst,
	LEQ:     leqConst,
	GEQ:     geqConst,
}

func addConst(x Const, y Const) Const {
	var c Const
	kind := x.kind
	switch kind {
	case Float32:
		c = ConstFloat32(x.Float32() + y.Float32())
	case Float64:
		c = ConstFloat64(x.Float64() + y.Float64())
	case Complex64:
		c = ConstComplex64(x.Complex64() + y.Complex64())
	case Complex128:
		c = ConstComplex128(x.Complex128() + y.Complex128())
	default:
		c = MakeConst(kind, truncateInt(ADD, kind, x.re+y.re))
	}
	return c
}

func subConst(x Const, y Const) Const {
	var c Const
	kind := x.kind
	switch kind {
	case Float32:
		c = ConstFloat32(x.Float32() - y.Float32())
	case Float64:
		c = ConstFloat64(x.Float64() - y.Float64())
	case Complex64:
		c = ConstComplex64(x.Complex64() - y.Complex64())
	case Complex128:
		c = ConstComplex128(x.Complex128() - y.Complex128())
	default:
		c = MakeConst(kind, truncateInt(SUB, kind, x.re-y.re))
	}
	return c
}

func mulConst(x Const, y Const) Const {
	var c Const
	kind := x.kind
	switch kind {
	case Float32:
		c = ConstFloat32(x.Float32() * y.Float32())
	case Float64:
		c = ConstFloat64(x.Float64() * y.Float64())
	case Complex64:
		c = ConstComplex64(x.Complex64() * y.Complex64())
	case Complex128:
		c = ConstComplex128(x.Complex128() * y.Complex128())
	default:
		c = MakeConst(kind, truncateInt(MUL, kind, x.re*y.re))
	}
	return c
}

func quoConst(x Const, y Const) Const {
	var c Const
	kind := x.kind
	switch kind {
	case Float32:
		c = ConstFloat32(x.Float32() / y.Float32())
	case Float64:
		c = ConstFloat64(x.Float64() / y.Float64())
	case Complex64:
		c = ConstComplex64(x.Complex64() / y.Complex64())
	case Complex128:
		c = ConstComplex128(x.Complex128() / y.Complex128())
	default:
		c = MakeConst(kind, truncateInt(QUO, kind, x.re/y.re))
	}
	return c
}

func remConst(x Const, y Const) Const {
	return MakeConst(x.kind, truncateInt(REM, x.kind, x.re%y.re))
}

func andConst(x Const, y Const) Const {
	return MakeConst(x.kind, truncateInt(AND, x.kind, x.re&y.re))
}

func orConst(x Const, y Const) Const {
	return MakeConst(x.kind, truncateInt(OR, x.kind, x.re|y.re))
}

func xorConst(x Const, y Const) Const {
	return MakeConst(x.kind, truncateInt(XOR, x.kind, x.re^y.re))
}

func shlConst(x Const, y Const) Const {
	return MakeConst(x.kind, truncateInt(SHL, x.kind, x.re<<uint8(y.re)))
}

func shrConst(x Const, y Const) Const {
	var val int64
	if x.kind.IsSigned() {
		val = x.re >> uint8(y.re)
	} else {
		val = int64(uint64(x.re) >> uint8(y.re))
	}
	return MakeConst(x.kind, truncateInt(SHR, x.kind, val))
}

func andNotConst(x Const, y Const) Const {
	return MakeConst(x.kind, truncateInt(AND_NOT, x.kind, x.re&^y.re))
}

func landConst(x Const, y Const) Const {
	return MakeConst(x.kind, x.re&y.re&1)
}

func lorConst(x Const, y Const) Const {
	return MakeConst(x.kind, (x.re|y.re)&1)
}

func eqlConst(x Const, y Const) Const {
	var flag bool
	switch x.kind {
	case Float32:
		flag = x.Float32() == y.Float32()
	case Float64:
		flag = x.Float64() == y.Float64()
	case Complex64:
		flag = x.Complex64() == y.Complex64()
	case Complex128:
		flag = x.Complex128() == y.Complex128()
	default:
		flag = x.re == y.re
	}
	return ConstBool(flag)
}

func neqConst(x Const, y Const) Const {
	c := eqlConst(x, y)
	c.re ^= 1
	return c
}

func lssConst(x Const, y Const) Const {
	var flag bool
	switch x.kind {
	case Float32:
		flag = x.Float32() < y.Float32()
	case Float64:
		flag = x.Float64() < y.Float64()
	case Complex64, Complex128:
		BadOpKind(LSS, x.kind)
	default:
		if x.kind.IsSigned() {
			flag = x.re < y.re
		} else {
			flag = uint64(x.re) < uint64(y.re)
		}
	}
	return ConstBool(flag)
}

func gtrConst(x Const, y Const) Const {
	return lssConst(y, x)
}

func leqConst(x Const, y Const) Const {
	var flag bool
	switch x.kind {
	case Float32:
		flag = x.Float32() <= y.Float32()
	case Float64:
		flag = x.Float64() <= y.Float64()
	case Complex64, Complex128:
		BadOpKind(LEQ, x.kind)
	default:
		if x.kind.IsSigned() {
			flag = x.re <= y.re
		} else {
			flag = uint64(x.re) <= uint64(y.re)
		}
	}
	return ConstBool(flag)
}

func geqConst(x Const, y Const) Const {
	return leqConst(y, x)
}

func truncateInt(op Op, kind Kind, val int64) int64 {
	switch kind {
	case Int:
		if kind.Size() == 4 {
			val = int64(int32(val))
		}
	case Int8:
		val = int64(int8(val))
	case Int16:
		val = int64(int16(val))
	case Int32:
		val = int64(int32(val))
	case Int64:
		break
	case Uint, Uintptr, Ptr:
		if kind.Size() == 4 {
			val = int64(uint32(val))
		}
	case Uint8:
		val = int64(uint8(val))
	case Uint16:
		val = int64(uint16(val))
	case Uint32:
		val = int64(uint32(val))
	case Uint64:
		break
	default:
		BadOpKind(op, kind)
	}
	return val
}

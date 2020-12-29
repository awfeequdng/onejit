/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * compile_binary.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/onejit/jit/internal"
)

// true if BinaryExpr can be compiled as memory operand of LEA
func canCompileAsLea(e *BinaryExpr) bool {
	var ret bool
	if e.Kind().IsIntegerOrPtr() {
		y := e.Y()
		switch e.Op() {
		case ADD:
			ret = true
		case SUB:
			ret = y.IsConst()
		case MUL:
			if y.IsConst() {
				switch y.(Const).Int() {
				case 1, 2, 3, 4, 5, 8, 9:
					ret = true
				}
			}
		}
	}
	return ret
}

// compile BinaryExpr as memory operand of LEA
func compileAsLea(e *BinaryExpr, ac *Asm) Expr {
	KindMustBeIntegerOrPtr(X86_LEA, e.Kind())
	x, y := e.X(), e.Y()

	switch e.Op() {
	case ADD:
		if xe, ok := x.(*BinaryExpr); ok && xe.Op() == MUL {
			// may be x*scale + base_or_offset
			return compileAsLea3(xe.X(), xe.Y(), y, ac)
		} else if ye, ok := y.(*BinaryExpr); ok && ye.Op() == MUL {
			// may be base_or_offset + x*scale
			return compileAsLea3(ye.X(), ye.Y(), x, ac)
		}
		base := compileReg(x, ac)
		y = compileRegOrConst(y, ac)
		return makeLea2(base, y)
	case SUB:
		base := compileReg(x, ac)

		c := y.(Const)
		c = MakeConst(c.Kind(), -c.Int())
		y = compileConst(c, ac)
		return makeLea2(base, y)

	case MUL: // x*scale
		var base, index Reg
		var scale uint8
		reg := compileReg(x, ac)
		c := compileConst(y.(Const), ac).(Const)

		switch val := c.Int(); val {
		case 1: // should be already optimized away...
			base = reg
		case 2, 4, 8:
			index = reg
			scale = uint8(val)
		case 3, 5, 9:
			base = reg
			index = reg
			scale = uint8(val - 1)
		}
		return makeLea3(base, index, scale)
	}
	return e
}

func compileAsLea3(eindex Expr, escale Expr, eoffset Expr, ac *Asm) Amd64Mem {
	var base, index Reg
	var scale uint8
	var offset int32

	eindex = compileRegOrConst(eindex, ac)
	escale = compileRegOrConst(escale, ac)
	eoffset = compileRegOrConst(eoffset, ac)
	if eindex.Class() == CONSTANT && escale.Class() != CONSTANT {
		eindex, escale = escale, eindex
	}
	if c, ok := escale.(Const); ok {
		switch val := c.Int(); val {
		case 1, 2, 4, 8:
			index = eindex.(Reg)
			scale = uint8(val)
		case 3, 5, 9:
			base = eindex.(Reg)
			index = base
			scale = uint8(val) - 1
		}
	}
	if scale == 0 {
		index = SpillToReg(Binary(MUL, eindex, escale), ac)
		scale = 1
	}
	if c, ok := eoffset.(Const); ok {
		offset = int32(c.Int())
	} else {
		if base.RegId() != NoRegId {
			// base is already occupied, spill it
			index = SpillToReg(
				Unary(X86_LEA,
					MakeAmd64Mem(Uintptr, 0, base.RegId(), index.RegId(), scale)),
				ac)
			scale = 1
		}
		base = eoffset.(Reg)
	}
	return MakeAmd64Mem(Uintptr, offset, base.RegId(), index.RegId(), scale)
}

func makeLea2(base Reg, offsetOrIndex Expr) Amd64Mem {
	if c, ok := offsetOrIndex.(Const); ok {
		offset, ok := toInt32(c)
		if !ok {
			Errorf("makeLea2 internal error: constant %v overflows int32", c.Int())
		}
		return MakeAmd64Mem(Uintptr, offset, base.RegId(), NoRegId, 0)
	}
	return MakeAmd64Mem(Uintptr, 0, base.RegId(), offsetOrIndex.RegId(), 1)
}

func makeLea3(base Reg, index Reg, scale uint8) Amd64Mem {
	return MakeAmd64Mem(Uintptr, 0, base.RegId(), index.RegId(), scale)
}

/*
 * gomacrojit - JIT compiler in Go
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
	. "github.com/cosmos72/gomacrojit/jit/internal"
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
func compileAsLea(e *BinaryExpr, ac *ArchCompiled) Expr {
	KindMustBeIntegerOrPtr(X86_LEA, e.Kind())
	x, y := e.X(), e.Y()

	switch e.Op() {
	case ADD:
		// TODO: x*scale + offset
		reg := compileReg(x, ac)
		y = compileRegOrConst(y, ac)
		return makeLea2(reg, y)
	case SUB:
		reg := compileReg(x, ac)

		c := y.(Const)
		c = MakeConst(c.Kind(), -c.Int())
		y = compileConst(c, ac)
		return makeLea2(reg, y)
	}
	return e
}

func makeLea2(x Reg, y Expr) Amd64Mem {
	if c, ok := y.(Const); ok {
		offset, ok := toInt32(c)
		if !ok {
			Errorf("makeLea2 internal error: constant %v overflows int32", c.Int())
		}
		return MakeAmd64Mem(Uintptr, offset, x.RegId(), NoRegId, 0)
	}
	return MakeAmd64Mem(Uintptr, 0, x.RegId(), y.RegId(), 1)
}

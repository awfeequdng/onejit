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
 * asm.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

// ================================== Asm ========================================

type Asm Compiled

func (ac *Asm) Func() *Func {
	return ac.fun
}

func (ac *Asm) Add(e Expr) *Asm {
	ac.code = append(ac.code, e)
	return ac
}

func (ac *Asm) Children() int {
	return len(ac.code)
}

func (ac *Asm) Child(i int) Node {
	return ac.code[i]
}

func SpillToReg(e Expr, ac *Asm) Reg {
	var reg Reg
	switch e := e.(type) {
	case Reg:
		reg = e
	case Const:
		reg = e.spillToReg(ac)
	case Label:
		reg = e.spillToReg(ac)
	default:
		reg = ac.Func().NewReg(e.Kind())
		ac.Add(Binary(ASSIGN, reg, e))
	}
	return reg
}

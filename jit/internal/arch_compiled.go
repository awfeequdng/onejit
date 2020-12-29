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
 * arch_compiled.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

// ================================== ArchCompiled =============================

type ArchCompiled Compiled

func (ac *ArchCompiled) Func() *Func {
	return ac.fun
}

func (ac *ArchCompiled) Add(e Expr) *ArchCompiled {
	ac.code = append(ac.code, e)
	return ac
}

func (ac *ArchCompiled) Children() int {
	return len(ac.code)
}

func (ac *ArchCompiled) Child(i int) Node {
	return ac.code[i]
}

func SpillToReg(e Expr, ac *ArchCompiled) Reg {
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

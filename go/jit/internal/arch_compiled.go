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

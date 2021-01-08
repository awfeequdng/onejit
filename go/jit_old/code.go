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
 * code.go
 *
 *  Created on Feb 10, 2019
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/onejit/go/jit_old/asm"
)

// will be passed as argument to asm.Asm()
type Code []AsmCode

func (c *Code) Init() *Code {
	*c = nil
	return c
}

func (c *Code) Op1(op Op1, src Expr, dst Expr) *Code {
	*c = append(*c, op.Asm(), asmArg(src), asmArg(dst))
	return c
}

func (c *Code) Op2(op Op2, a Expr, b Expr, dst Expr) *Code {
	*c = append(*c, op.Asm(), asmArg(a), asmArg(b), asmArg(dst))
	return c
}

func (c *Code) Inst1(inst Inst1, dst Expr) *Code {
	if inst != NOP {
		*c = append(*c, inst.Asm(), asmArg(dst))
	}
	return c
}

// destination is first argument, as Comp.Stmt2
func (c *Code) Inst2(inst Inst2, dst Expr, src Expr) *Code {
	adst, asrc := asmArg(dst), asmArg(src)
	if inst != ASSIGN || asrc != adst {
		*c = append(*c, inst.Asm(), asrc, adst)
	}
	return c
}

// destination is first argument, as Comp.Stmt3
func (c *Code) Inst3(inst Inst3, dst Expr, darg Expr, src Expr) *Code {
	adst, adarg, asrc := asmArg(dst), asmArg(darg), asmArg(src)
	// asm.SETIDX arguments are dst, doffset, src
	*c = append(*c, inst.Asm(), adst, adarg, asrc)
	return c
}

// ALLOC/FREE
func (c *Code) SoftReg(op asm.Op1Misc, s SoftReg) *Code {
	*c = append(*c, op, s)
	return c
}

func asmArg(e Expr) AsmCode {
	switch e := e.(type) {
	case AsmCode:
		return e
	case SoftReg:
		return e
	case *Expr1, *Expr2:
		errorf("internal error: cannot assemble %T, must be compiled first: %v", e, e)
	default:
		errorf("unknown Expr type %T: %v", e, e)
	}
	return nil
}

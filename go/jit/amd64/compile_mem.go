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
 * compile.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/onejit/go/jit/internal"
)

func compileMem(m Mem, ac *Asm) Expr {
	var ret Amd64Mem
	// kind := m.Kind()
	switch addr := m.Addr().(type) {
	case Const:
		if offset, ok := toInt32(addr); ok {
			ret = MakeAmd64Mem(m.Kind(), offset, NoRegId, NoRegId, 0)
		} else {
			reg := SpillToReg(m.Addr(), ac)
			ret = MakeAmd64Mem(m.Kind(), 0, reg.RegId(), NoRegId, 0)
		}
	case Reg:
		KindMustBeUintptrOrPtr("Amd64Mem", addr.Kind())
		ret = MakeAmd64Mem(m.Kind(), 0, addr.RegId(), NoRegId, 0)
	case *BinaryExpr:
		switch addr.Op() {
		case ADD:
			// return addToAmd64Mem(f, expr.X(), expr.Y())
		case SUB:
			// return addToAmd64Mem(f, expr.X(), expr.Y())
		}
		Warnf("unimplemented amd64.compileMem() for address type %T", addr)
		return m
	default:
		Warnf("unimplemented amd64.compileMem() for address type %T", addr)
		return m
	}
	return ret
}

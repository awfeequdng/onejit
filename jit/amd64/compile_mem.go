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
 * compile.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/onejit/jit/internal"
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

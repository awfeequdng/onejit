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
 * dsl.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package common

func (asm *Asm) Assemble(args ...AsmCode) *Asm {
	n := len(args)
	for i := 0; i < n; i++ {
		i += asm.Op(args[i:]...)
	}
	return asm
}

func (asm *Asm) Op(args ...AsmCode) int {
	var n int
	switch op := args[0].(type) {
	case Op0:
		asm.Op0(op)
		n = 0
	case Op1:
		asm.Op1(op, asm.Arg(args[1]))
		n = 1
	case Op1Misc:
		asm.Op1Misc(op, args[1])
		n = 1
	case Op2Misc:
		asm.Op2Misc(op, args[1], args[2])
		n = 2
	case Op2:
		asm.Op2(op, asm.Arg(args[1]), asm.Arg(args[2]))
		n = 2
	case Op3:
		asm.Op3(op, asm.Arg(args[1]), asm.Arg(args[2]), asm.Arg(args[3]))
		n = 3
	case Op4:
		asm.Op4(op, asm.Arg(args[1]), asm.Arg(args[2]), asm.Arg(args[3]), asm.Arg(args[4]))
		n = 4
	default:
		errorf("syntax error: expecting Op0,Op1,Op1Misc,Op2Misc,Op2,Op3 or Op4 [args], found %v // %T", op, op)
	}
	return n
}

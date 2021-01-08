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
 * op0.go
 *
 *  Created on Jan 27, 2019
 *      Author Massimiliano Ghilardi
 */

package arm64

// ============================================================================
// no-arg instruction

func op0val(op Op0) uint32 {
	var val uint32
	switch op {
	case BAD:
		val = 0x00000000
	case NOP:
		val = 0xD503201F
	case RET:
		val = 0xD65F03C0
	default:
		errorf("unknown Op0 instruction: %v", op)
	}
	return val
}

// ============================================================================
func (arch Arm64) Op0(asm *Asm, op Op0) *Asm {
	return asm.Uint32(op0val(op))
}

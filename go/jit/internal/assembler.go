/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
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
 * assembler.go
 *
 *  Created on May 24, 2018
 *      Author Massimiliano Ghilardi
 */

package internal

type Assembler struct {
	code []byte
	// map from label to offset in code[]
	labels map[Label]int
}

func NewAssembler() *Assembler {
	return &Assembler{
		nil,
		nil,
	}
}

func (asm *Assembler) Clear() *Assembler {
	asm.code = nil
	asm.labels = nil
	return asm
}

func (asm *Assembler) Byte(b byte) *Assembler {
	asm.code = append(asm.code, b)
	return asm
}

func (asm *Assembler) Bytes(bytes ...byte) *Assembler {
	asm.code = append(asm.code, bytes...)
	return asm
}

func (asm *Assembler) Uint8(val uint8) *Assembler {
	asm.code = append(asm.code, val)
	return asm
}

func (asm *Assembler) Uint16(val uint16) *Assembler {
	asm.code = append(asm.code, uint8(val), uint8(val>>8))
	return asm
}

func (asm *Assembler) Uint32(val uint32) *Assembler {
	asm.code = append(asm.code, uint8(val), uint8(val>>8), uint8(val>>16), uint8(val>>24))
	return asm
}

func (asm *Assembler) Uint64(val uint64) *Assembler {
	asm.code = append(asm.code,
		uint8(val), uint8(val>>8), uint8(val>>16), uint8(val>>24),
		uint8(val>>32), uint8(val>>40), uint8(val>>48), uint8(val>>56))
	return asm
}

func (asm *Assembler) Int8(val int8) *Assembler {
	return asm.Uint8(uint8(val))
}

func (asm *Assembler) Int16(val int16) *Assembler {
	return asm.Uint16(uint16(val))
}

func (asm *Assembler) Int32(val int32) *Assembler {
	return asm.Uint32(uint32(val))
}

func (asm *Assembler) Int64(val int64) *Assembler {
	return asm.Uint64(uint64(val))
}

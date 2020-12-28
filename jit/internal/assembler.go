/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
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

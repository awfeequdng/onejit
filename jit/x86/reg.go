/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * reg.go
 *
 *  Created on Dec 26, 2020
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/gomacrojit/jit"
	"github.com/cosmos72/gomacrojit/jit/amd64"
)

// ============================ RegId ==========================================
const (
	EAX  = amd64.RAX
	ECX  = amd64.RCX
	EDX  = amd64.RDX
	EBX  = amd64.RBX
	ESP  = amd64.RSP
	EBP  = amd64.RBP
	ESI  = amd64.RSI
	EDI  = amd64.RDI
	XMM0 = amd64.XMM0
	XMM1 = amd64.XMM1
	XMM2 = amd64.XMM2
	XMM3 = amd64.XMM3
	XMM4 = amd64.XMM4
	XMM5 = amd64.XMM5
	XMM6 = amd64.XMM6
	XMM7 = amd64.XMM7
	RLo  = EAX
	RHi  = XMM7
)

func ValidateRegId(id RegId) {
	if id < RLo || id > RHi || (id > EDI && id < XMM0) {
		Errorf("invalid register id: %v", int(id))
	}
}

func bits(id RegId) uint8 {
	ValidateRegId(id)
	return uint8(id - 1)
}

/*
// return number of assembler bytes needed to encode m.off
func offlen(m Mem, id RegId) (offlen uint8, offbit uint8) {
	moffset := m.Offset()
	switch {
	// (%rbp) and (%r13) registers must use 1-byte offset even if m.off == 0
	case moffset == 0 && id != RBP && id != R13:
		return 0, 0
	case moffset == int32(int8(moffset)):
		return 1, 0x40
	default:
		return 4, 0x80
	}
}

func quirk24(asm *Asm, id RegId) *Asm {
	if id == RSP || id == R12 {
		asm.Bytes(0x24) // amd64 quirk
	}
	return asm
}
*/

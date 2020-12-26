/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
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
	"fmt"

	. "github.com/cosmos72/gomacrojit/jit"
)

// ============================================================================
// register
const (
	noregid RegId = NoRegId + iota
	RAX
	RCX
	RDX
	RBX
	RSP
	RBP
	RSI
	RDI
	R8
	R9
	R10
	R11
	R12
	R13
	R14
	R15
	XMM0
	XMM1
	XMM2
	XMM3
	XMM4
	XMM5
	XMM6
	XMM7
	XMM8
	XMM9
	XMM10
	XMM11
	XMM12
	XMM13
	XMM14
	XMM15
	RLo RegId = RAX
	RHi RegId = XMM15
)

var regName1 = [33]string{
	RAX:   "%al",
	RCX:   "%cl",
	RDX:   "%dl",
	RBX:   "%bl",
	RSP:   "%spl",
	RBP:   "%bpl",
	RSI:   "%sil",
	RDI:   "%dil",
	R8:    "%r8b",
	R9:    "%r9b",
	R10:   "%r10b",
	R11:   "%r11b",
	R12:   "%r12b",
	R13:   "%r13b",
	R14:   "%r14b",
	R15:   "%r15b",
	XMM0:  "%xmm0b",
	XMM1:  "%xmm1b",
	XMM2:  "%xmm2b",
	XMM3:  "%xmm3b",
	XMM4:  "%xmm4b",
	XMM5:  "%xmm5b",
	XMM6:  "%xmm6b",
	XMM7:  "%xmm7b",
	XMM8:  "%xmm8b",
	XMM9:  "%xmm9b",
	XMM10: "%xmm10b",
	XMM11: "%xmm11b",
	XMM12: "%xmm12b",
	XMM13: "%xmm13b",
	XMM14: "%xmm14b",
	XMM15: "%xmm15b",
}
var regName2 = [33]string{
	RAX:   "%ax",
	RCX:   "%cx",
	RDX:   "%dx",
	RBX:   "%bx",
	RSP:   "%sp",
	RBP:   "%bp",
	RSI:   "%si",
	RDI:   "%di",
	R8:    "%r8w",
	R9:    "%r9w",
	R10:   "%r10w",
	R11:   "%r11w",
	R12:   "%r12w",
	R13:   "%r13w",
	R14:   "%r14w",
	R15:   "%r15w",
	XMM0:  "%xmm0w",
	XMM1:  "%xmm1w",
	XMM2:  "%xmm2w",
	XMM3:  "%xmm3w",
	XMM4:  "%xmm4w",
	XMM5:  "%xmm5w",
	XMM6:  "%xmm6w",
	XMM7:  "%xmm7w",
	XMM8:  "%xmm8w",
	XMM9:  "%xmm9w",
	XMM10: "%xmm10w",
	XMM11: "%xmm11w",
	XMM12: "%xmm12w",
	XMM13: "%xmm13w",
	XMM14: "%xmm14w",
	XMM15: "%xmm15w",
}
var regName4 = [33]string{
	RAX:   "%eax",
	RCX:   "%ecx",
	RDX:   "%edx",
	RBX:   "%ebx",
	RSP:   "%esp",
	RBP:   "%ebp",
	RSI:   "%esi",
	RDI:   "%edi",
	R8:    "%r8d",
	R9:    "%r9d",
	R10:   "%r10d",
	R11:   "%r11d",
	R12:   "%r12d",
	R13:   "%r13d",
	R14:   "%r14d",
	R15:   "%r15d",
	XMM0:  "%xmm0d",
	XMM1:  "%xmm1d",
	XMM2:  "%xmm2d",
	XMM3:  "%xmm3d",
	XMM4:  "%xmm4d",
	XMM5:  "%xmm5d",
	XMM6:  "%xmm6d",
	XMM7:  "%xmm7d",
	XMM8:  "%xmm8d",
	XMM9:  "%xmm9d",
	XMM10: "%xmm10d",
	XMM11: "%xmm11d",
	XMM12: "%xmm12d",
	XMM13: "%xmm13d",
	XMM14: "%xmm14d",
	XMM15: "%xmm15d",
}
var regName8 = [33]string{
	RAX:   "%rax",
	RCX:   "%rcx",
	RDX:   "%rdx",
	RBX:   "%rbx",
	RSP:   "%rsp",
	RBP:   "%rbp",
	RSI:   "%rsi",
	RDI:   "%rdi",
	R8:    "%r8",
	R9:    "%r9",
	R10:   "%r10",
	R11:   "%r11",
	R12:   "%r12",
	R13:   "%r13",
	R14:   "%r14",
	R15:   "%r15",
	XMM0:  "%xmm0q",
	XMM1:  "%xmm1q",
	XMM2:  "%xmm2q",
	XMM3:  "%xmm3q",
	XMM4:  "%xmm4q",
	XMM5:  "%xmm5q",
	XMM6:  "%xmm6q",
	XMM7:  "%xmm7q",
	XMM8:  "%xmm8q",
	XMM9:  "%xmm9q",
	XMM10: "%xmm10q",
	XMM11: "%xmm11q",
	XMM12: "%xmm12q",
	XMM13: "%xmm13q",
	XMM14: "%xmm14q",
	XMM15: "%xmm15q",
}
var regName16 = [33]string{
	XMM0:  "%xmm0",
	XMM1:  "%xmm1",
	XMM2:  "%xmm2",
	XMM3:  "%xmm3",
	XMM4:  "%xmm4",
	XMM5:  "%xmm5",
	XMM6:  "%xmm6",
	XMM7:  "%xmm7",
	XMM8:  "%xmm8",
	XMM9:  "%xmm9",
	XMM10: "%xmm10",
	XMM11: "%xmm11",
	XMM12: "%xmm12",
	XMM13: "%xmm13",
	XMM14: "%xmm14",
	XMM15: "%xmm15",
}

func RegIdValid(id RegId) bool {
	return id >= RLo && id <= RHi
}

func RegIdValidate(id RegId) {
	if !RegIdValid(id) {
		Errorf("invalid register id: %d", int(id))
	}
}

func RegIdString(id RegId) string {
	var s string
	if RegIdValid(id) {
		if id < XMM0 {
			s = regName8[id]
		} else {
			s = regName16[id]
		}
	} else {
		s = fmt.Sprintf("%%badregid:%d", int(id))
	}
	return s
}

func RegString(r Reg) string {
	id := r.RegId()
	var s string
	if RegIdValid(id) {
		switch r.Kind().Size() {
		case 1:
			s = regName1[id]
		case 2:
			s = regName2[id]
		case 4:
			s = regName4[id]
		case 8:
			s = regName8[id]
		case 16:
			s = regName16[id]
		}
	}
	if len(s) == 0 {
		s = fmt.Sprintf("%%badreg:%d%s", int(id), r.Kind().SizeString())
	}
	return s
}

func bits(id RegId) uint8 {
	RegIdValidate(id)
	return uint8(id-1) & 15
}

func lohiId(id RegId) (uint8, uint8) {
	bits := bits(id)
	return bits & 0x7, (bits & 0x8) >> 3
}

func lohi(r Reg) (uint8, uint8) {
	return lohiId(r.RegId())
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

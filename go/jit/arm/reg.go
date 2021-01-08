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
 * reg.go
 *
 *  Created on Dec 26, 2020
 *      Author Massimiliano Ghilardi
 */

package arm

import (
	"fmt"

	. "github.com/cosmos72/onejit/go/jit"
)

// ============================================================================
// register
const (
	noregid RegId = NoRegId + iota
	R0
	R1
	R2
	R3
	R4
	R5
	R6
	R7
	R8
	R9
	R10
	R11
	R12
	R13
	R14
	R15
	D0
	D1
	D2
	D3
	D4
	D5
	D6
	D7
	D8
	D9
	D10
	D11
	D12
	D13
	D14
	D15
	D16
	D17
	D18
	D19
	D20
	D21
	D22
	D23
	D24
	D25
	D26
	D27
	D28
	D29
	D30
	D31
	RLo RegId = R0
	RHi RegId = D31
)

var regName = [17]string{
	R0:  "r0",
	R1:  "r1",
	R2:  "r2",
	R3:  "r3",
	R4:  "r4",
	R5:  "r5",
	R6:  "r6",
	R7:  "r7",
	R8:  "r8",
	R9:  "r9",
	R10: "r10",
	R11: "r11",
	R12: "r12",
	R13: "r13",
	R14: "r14",
	R15: "r15",
}

// go:linkname regNameNEON github.com/cosmos72/onejit/jit/arm64.regname8
var regNameNEON [33]string

var regNameNEONQ = [16]string{
	"Q0",
	"Q1",
	"Q2",
	"Q3",
	"Q4",
	"Q5",
	"Q6",
	"Q7",
	"Q8",
	"Q9",
	"Q10",
	"Q11",
	"Q12",
	"Q13",
	"Q14",
	"Q15",
}

func RegIdValid(id RegId) bool {
	return id >= RLo && id <= RHi && (id <= R15 || id >= D0)
}

func RegIdValidate(id RegId) {
	if !RegIdValid(id) {
		Errorf("invalid register id: 0x%x", int(id))
	}
}

func RegIdString(id RegId) string {
	var s string
	if RegIdValid(id) {
		if id <= R15 {
			s = regName[id]
		} else {
			s = regNameNEON[id]
		}
	} else {
		s = fmt.Sprintf("badregid:%x", int(id))
	}
	return s
}

func RegString(r Reg) string {
	id := r.RegId()
	var s string
	if RegIdValid(id) {
		if id <= R15 {
			s = regName[id]
		} else {
			switch r.Kind().Size() {
			case 1, 2, 4, 8:
				s = regNameNEON[id]
			case 16:
				if b := bits(id); b&1 == 0 {
					s = regNameNEONQ[b/2]
				}
			}
		}
	}
	if len(s) == 0 {
		s = fmt.Sprintf("badreg:%x%s", int(id), r.Kind().SizeString())
	}
	return s
}

func bits(id RegId) uint8 {
	RegIdValidate(id)
	return uint8(id-1) & 31
}

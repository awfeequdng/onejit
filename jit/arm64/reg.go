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
 * reg.go
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	"fmt"

	. "github.com/cosmos72/onejit/jit"
)

// ============================================================================
// register
const (
	noregid RegId = NoRegId + iota
	X0
	X1
	X2
	X3
	X4
	X5
	X6
	X7
	X8
	X9
	X10
	X11
	X12
	X13
	X14
	X15
	X16
	X17
	X18
	X19
	X20
	X21
	X22
	X23
	X24
	X25
	X26
	X27
	X28
	X29
	X30
	X31
	V0
	V1
	V2
	V3
	V4
	V5
	V6
	V7
	V8
	V9
	V10
	V11
	V12
	V13
	V14
	V15
	V16
	V17
	V18
	V19
	V20
	V21
	V22
	V23
	V24
	V25
	V26
	V27
	V28
	V29
	V30
	V31
	RLo RegId = X0
	RHi RegId = V31
)

var regName4 = [...]string{
	X0:  "w0",
	X1:  "w1",
	X2:  "w2",
	X3:  "w3",
	X4:  "w4",
	X5:  "w5",
	X6:  "w6",
	X7:  "w7",
	X8:  "w8",
	X9:  "w9",
	X10: "w10",
	X11: "w11",
	X12: "w12",
	X13: "w13",
	X14: "w14",
	X15: "w15",
	X16: "w16",
	X17: "w17",
	X18: "w18",
	X19: "w19",
	X20: "w20",
	X21: "w21",
	X22: "w22",
	X23: "w23",
	X24: "w24",
	X25: "w25",
	X26: "w26",
	X27: "w27",
	X28: "w28",
	X29: "w29",
	X30: "w30",
	X31: "w31",
	V0:  "s0",
	V1:  "s1",
	V2:  "s2",
	V3:  "s3",
	V4:  "s4",
	V5:  "s5",
	V6:  "s6",
	V7:  "s7",
	V8:  "s8",
	V9:  "s9",
	V10: "s10",
	V11: "s11",
	V12: "s12",
	V13: "s13",
	V14: "s14",
	V15: "s15",
	V16: "s16",
	V17: "s17",
	V18: "s18",
	V19: "s19",
	V20: "s20",
	V21: "s21",
	V22: "s22",
	V23: "s23",
	V24: "s24",
	V25: "s25",
	V26: "s26",
	V27: "s27",
	V28: "s28",
	V29: "s29",
	V30: "s30",
	V31: "s31",
}
var regName8 = [...]string{
	X0:  "x0",
	X1:  "x1",
	X2:  "x2",
	X3:  "x3",
	X4:  "x4",
	X5:  "x5",
	X6:  "x6",
	X7:  "x7",
	X8:  "x8",
	X9:  "x9",
	X10: "x10",
	X11: "x11",
	X12: "x12",
	X13: "x13",
	X14: "x14",
	X15: "x15",
	X16: "x16",
	X17: "x17",
	X18: "x18",
	X19: "x19",
	X20: "x20",
	X21: "x21",
	X22: "x22",
	X23: "x23",
	X24: "x24",
	X25: "x25",
	X26: "x26",
	X27: "x27",
	X28: "x28",
	X29: "x29",
	X30: "x30",
	X31: "x31",
	V0:  "d0",
	V1:  "d1",
	V2:  "d2",
	V3:  "d3",
	V4:  "d4",
	V5:  "d5",
	V6:  "d6",
	V7:  "d7",
	V8:  "d8",
	V9:  "d9",
	V10: "d10",
	V11: "d11",
	V12: "d12",
	V13: "d13",
	V14: "d14",
	V15: "d15",
	V16: "d16",
	V17: "d17",
	V18: "d18",
	V19: "d19",
	V20: "d20",
	V21: "d21",
	V22: "d22",
	V23: "d23",
	V24: "d24",
	V25: "d25",
	V26: "d26",
	V27: "d27",
	V28: "d28",
	V29: "d29",
	V30: "d30",
	V31: "d31",
}
var regName16 = [...]string{
	V0:  "v0",
	V1:  "v1",
	V2:  "v2",
	V3:  "v3",
	V4:  "v4",
	V5:  "v5",
	V6:  "v6",
	V7:  "v7",
	V8:  "v8",
	V9:  "v9",
	V10: "v10",
	V11: "v11",
	V12: "v12",
	V13: "v13",
	V14: "v14",
	V15: "v15",
	V16: "v16",
	V17: "v17",
	V18: "v18",
	V19: "v19",
	V20: "v20",
	V21: "v21",
	V22: "v22",
	V23: "v23",
	V24: "v24",
	V25: "v25",
	V26: "v26",
	V27: "v27",
	V28: "v28",
	V29: "v29",
	V30: "v30",
	V31: "v31",
}

func RegIdValid(id RegId) bool {
	return id >= RLo && id <= RHi
}

func RegIdValidate(id RegId) {
	if !RegIdValid(id) {
		Errorf("invalid register id: 0x%x", int(id))
	}
}

func RegIdString(id RegId) string {
	var s string
	if RegIdValid(id) {
		if id >= V0 {
			s = regName16[id]
		} else {
			s = regName8[id]
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
		switch r.Kind().Size() {
		case 1, 2, 4:
			s = regName4[id]
		case 8:
			s = regName8[id]
		case 16:
			s = regName16[id]
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

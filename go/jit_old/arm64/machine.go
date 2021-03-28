/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * machine.go
 *
 *  Created on May 26, 2018
 *      Author Massimiliano Ghilardi
 */

package arm64

import (
	"fmt"
)

const (
	noregid = RegId(ARM64-1)<<8 + iota
	// integer registers
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
	// depending on context, zero register or stack pointer
	XZR, XSP = RegId(ARM64-1)<<8 + iota, RegId(ARM64-1)<<8 + iota
	// floating-point registers
	D0 = RegId(ARM64-1)<<8 + iota
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
	RLo = X0
	RHi = D31
	// stack pointer
	RSP = XSP
	// suggested register to point to local variables
	RVAR = X29
)

var (
	regName32 = makeRegNames("w", "s")
	regName64 = makeRegNames("x", "d")
)

func makeRegNames(prefixInt string, prefixFloat string) []string {
	name := make([]string, RHi+1)
	for id := X0; id <= X30; id++ {
		name[id] = fmt.Sprint(prefixInt, int(id)-1)
	}
	name[XZR] = prefixInt + "zr"
	for id := D0; id <= D31; id++ {
		name[id] = fmt.Sprint(prefixFloat, int(id)-1)
	}
	return name
}

// validate and return uint32 mask representing
// floating-point register r.id
func fval(r Reg) uint32 {
	r.Validate()
	assert(r.RegId().Kind().IsFloat())
	return uint32(r.RegId() - D0)
}

// return the bitmask to be or-ed to the instruction
// to specify the floating-point registers width
func fbit(r Reg) uint32 {
	return uint32(r.Kind().Size()) & 8 << 19
}

// validate and return uint32 mask representing
// integer register r.id
// note that XSP/XZR is not considered valid
func kval(r Reg) uint32 {
	r.Validate()
	assert(!r.RegId().Kind().IsFloat())
	return uint32(r.RegId() - X0)
}

// validate and return uint32 mask representing r.id
// if allowX31 is true, also allows r.id == XSP/XZR
func valOrX31(id RegId, allowX31 bool) uint32 {
	if !allowX31 || id != XZR {
		id.Validate()
	}
	return uint32(id - X0)
}

// return the bitmask to be or-ed to the instruction
// to specify the integer registers width
func kbit(r Reg) uint32 {
	return uint32(r.Kind().Size()) & 8 << 28
}

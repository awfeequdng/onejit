/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2020 Massimiliano Ghilardi
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

package amd64

import (
	"fmt"

	. "github.com/cosmos72/onejit/go/jit"
	"github.com/cosmos72/onejit/go/jit/amd64"
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

// go:linkname regName github.com/cosmos72/onejit/jit/amd64.regname4
var regName [33]string

// go:linkname regNameXMM github.com/cosmos72/onejit/jit/amd64.regname16
var regNameXMM [33]string

func RegIdValid(id RegId) bool {
	return id >= RLo && id <= RHi && (id <= EDI || id >= XMM0)
}

func RegIdValidate(id RegId) {
	if !RegIdValid(id) {
		Errorf("invalid register id: %d", int(id))
	}
}

func RegIdString(id RegId) string {
	var s string
	if RegIdValid(id) {
		if id >= XMM0 {
			s = regNameXMM[id]
		} else {
			s = regName[id]
		}
	} else {
		s = fmt.Sprintf("%%badregid:%x", int(id))
	}
	return s
}

func RegString(r Reg) string {
	id := r.RegId()
	size := r.Kind().Size()
	var s string
	if RegIdValid(id) {
		if size <= 4 {
			s = amd64.RegString(r)
		} else if size == 8 && id >= XMM0 {
			s = regNameXMM[id]
		}
	}
	if len(s) == 0 {
		s = fmt.Sprintf("%%badreg:%x%s", int(id), r.Kind().SizeString())
	}
	return s
}

func bits(id RegId) uint8 {
	RegIdValidate(id)
	return uint8(id-1) & 7
}

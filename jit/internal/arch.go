/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * arch.go
 *
 *  Created on Feb 13, 2019
 *      Author Massimiliano Ghilardi
 */

package internal

import (
	"fmt"
)

type ArchId uint8

const (
	NOARCH ArchId = iota
	AMD64
	ARM64
	ARM
	X86
	archLo = NOARCH
	archHi = X86
)

type Arch struct {
	ArchId
	CpuWidth CpuWidth
	Compile  func(expr Expr, toplevel bool, ac *Asm) Expr
}

var Archs = map[ArchId]Arch{}

var archName = [...]string{
	NOARCH: "NOARCH",
	AMD64:  "AMD64",
	ARM64:  "ARM64",
	ARM:    "ARM",
	X86:    "X86",
}

func (archid ArchId) String() string {
	var s string
	if archid >= archLo && archid <= archHi {
		s = archName[archid]
	}
	if len(s) == 0 {
		s = fmt.Sprintf("ArchId(%d)", int(archid))
	}
	return s
}

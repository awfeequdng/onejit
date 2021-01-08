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

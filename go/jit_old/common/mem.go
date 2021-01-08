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
 * mem.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package common

import (
	"fmt"
)

// hardware memory location.
type Mem struct {
	off int32
	reg Reg // also defines kind, width and signedness
}

func (m Mem) String() string {
	arch := m.reg.id.Arch()
	if arch != nil {
		return arch.MemString(m)
	}
	return fmt.Sprintf("%v@{%v+%v}", m.reg.kind, m.reg.id, m.off)
}

// implement Arg interface
func (m Mem) RegId() RegId {
	return m.reg.id
}

func (m Mem) Kind() Kind {
	return m.reg.kind
}

func (m Mem) Const() bool {
	return false
}

func (m Mem) asmcode() {
}

func (m Mem) Offset() int32 {
	return m.off
}

func MakeMem(off int32, id RegId, kind Kind) Mem {
	return Mem{off: off, reg: Reg{id: id, kind: kind}}
}

/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
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
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

// memory address.
type Mem struct {
	kind Kind
	ro   bool // readonly?
	addr Expr
}

func (m Mem) IsAssignable() bool {
	return !m.ro
}

func MakeMem(kind Kind, addr Expr) Mem {
	KindMustBeUintptrOrPtr("Mem", addr.Kind())
	return Mem{
		kind: kind,
		ro:   false,
		addr: addr,
	}
}

// return a read-only view of a subset of memory contents
// note: memory does not become immutable - it can still be modified
// through the original struct
// note: the current implementation assumes little-endian!
func (m Mem) ReadOnly(subset Kind) Mem {
	KindMustBeSubset("Mem", subset, m.kind)
	return Mem{kind: subset, ro: true, addr: m.addr}
}

func (m Mem) Addr() Expr {
	return m.addr
}

// implement Expr interface
func (m Mem) Class() Class {
	return MEM
}

func (m Mem) Op() Op {
	return VAR
}

func (m Mem) Kind() Kind {
	return m.kind
}

func (m Mem) expr() {}

func (m Mem) RegId() RegId {
	return NoRegId
}

func (m Mem) Size() Size {
	return m.kind.Size()
}

func (m Mem) Children() int {
	return 1
}

func (m Mem) Child(i int) Node {
	if i == 0 {
		return m.addr
	}
	return BadIndex(i, 1)
}

func (m Mem) IsConst() bool {
	// memory access cannot be a compile-time constant
	return false
}

func (m Mem) IsPure() bool {
	// reading a memory address has the same side effects
	// as computing the address to be read
	return m.addr.IsPure()
}

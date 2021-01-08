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
 *  Created on May 24, 2018
 *      Author Massimiliano Ghilardi
 */

package internal

// machine register or local variable
type RegId uint16

const (
	NoRegId      RegId = 0
	MinSoftRegId RegId = 0x100
)

// machine register with kind and size
type Reg struct {
	id   RegId
	kind Kind
	ro   bool // readonly?
}

func (r Reg) IsAssignable() bool {
	return !r.ro
}

// return an architecture-specific Reg.
// used by subfolders
func archReg(kind Kind, id RegId) Reg {
	return Reg{id: id, kind: kind}
}

// return a dummy register with specified kind.
// writes to this register are discarded,
// and reads to this register always return zero
//
// among other things, it is used in a TupleExpr
// to discard some values of a multi-valued CallExpr
func NoReg(kind Kind) Reg {
	return Reg{kind: kind, id: NoRegId}
}

// return a read-only view of a subset of register bits
// note: register does not become immutable - it can still be modified
// through the original struct
func (r Reg) ReadOnly(subset Kind) Reg {
	KindMustBeSubset("Reg", subset, r.kind)
	return Reg{kind: subset, id: r.id, ro: true}
}

// implement Expr interface
func (r Reg) Class() Class {
	return REG
}

func (r Reg) Op() Op {
	return VAR
}

func (r Reg) Kind() Kind {
	return r.kind
}

func (r Reg) expr() {}

func (r Reg) RegId() RegId {
	return r.id
}

func (r Reg) Size() Size {
	return r.kind.Size()
}

func (r Reg) Children() int {
	return 0
}

func (r Reg) Child(i int) Node {
	return BadIndex(i, 0)
}

func (r Reg) IsConst() bool {
	return false
}

func (r Reg) IsPure() bool {
	// reading a register has no side effects
	return true
}

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
 *  Created on May 24, 2018
 *      Author Massimiliano Ghilardi
 */

package jit

// machine register or local variable
type RegId uint32

const (
	NoRegId      RegId = 0
	minSoftRegId RegId = 256
)

func (id RegId) ArchId() ArchId {
	return ArchId(1 + id>>24)
}

// machine register with kind and size
type Reg struct {
	kind Kind
	id   RegId
	ro   bool // readonly?
}

func (r Reg) IsAssignable() bool {
	return !r.ro
}

// return an architecture-specific Reg.
// used by subfolders
func archReg(kind Kind, id RegId) Reg {
	return Reg{kind: kind, id: id}
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
	kindMustBeSubset("Reg", subset, r.kind)
	return Reg{kind: subset, id: r.id, ro: true}
}

// implement Expr interface
func (r Reg) expr() {}

func (r Reg) RegId() RegId {
	return r.id
}

func (r Reg) IsConst() bool {
	return false
}

func (r Reg) Kind() Kind {
	return r.kind
}

func (r Reg) Size() Size {
	return r.kind.Size()
}

func (r Reg) Class() Class {
	return REG
}

func (r Reg) Children() int {
	return 0
}

func (r Reg) Child(i int) Node {
	return badIndex(i, 0)
}

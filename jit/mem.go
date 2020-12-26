/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * mem.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

// memory address.
type Mem struct {
	kind Kind
	ro   bool // readonly?
	addr Expr
}

func (m Mem) IsAssignable() bool {
	return !m.ro
}

// return a read-only view of a subset of memory contents
// note: memory does not become immutable - it can still be modified
// through the original struct
// note: the current implementation assumes little-endian!
func (m Mem) ReadOnly(subset Kind) Mem {
	kindMustBeSubset("Mem", subset, m.kind)
	return Mem{kind: subset, ro: true, addr: m.addr}
}

// implement Expr interface
func (m Mem) expr() {}

func (m Mem) RegId() RegId {
	return NoRegId
}

func (m Mem) Kind() Kind {
	return m.kind
}

func (m Mem) IsConst() bool {
	// memory access cannot be a compile-time constant
	return false
}

func (m Mem) Size() Size {
	return m.kind.Size()
}

func (m Mem) Class() Class {
	return MEM
}

func (m Mem) Children() int {
	return 1
}

func (m Mem) Child(i int) Node {
	if i == 0 {
		return m.addr
	}
	return badIndex(i, 1)
}

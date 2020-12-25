/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2019-2020 Massimiliano Ghilardi
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
	addr Expr
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
	// memory access cannot be a constant
	return false
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
	return badIndex(i, 1)
}

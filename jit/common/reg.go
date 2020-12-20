/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
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

package common

// machine register
type RegId uint16

const (
	NoRegId RegId = 0
)

func (id RegId) ArchId() ArchId {
	return ArchId(1 + id>>8)
}

// machine register with kind and size
type Reg struct {
	id   RegId
	kind Kind
}

// implement Expr interface
func (r Reg) expr() {}

func (r Reg) RegId() RegId {
	return r.id
}

func (r Reg) Const() bool {
	return false
}

func (r Reg) Kind() Kind {
	return r.kind
}

func (r Reg) Size() Size {
	return r.kind.Size()
}

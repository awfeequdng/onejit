/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2019 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * unary.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package common

type Unary struct {
	kind Kind
	op   Op
	x    Expr
}

// implement Expr interface
func (e Unary) expr() {}

func (e Unary) RegId() RegId {
	return NoRegId
}

func (e Unary) Kind() Kind {
	return e.kind
}

func (e Unary) Const() bool {
	// address dereference cannot be a constant
	return e.op != MUL && e.x.Const()
}

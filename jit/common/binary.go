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
 * binary.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package common

type Binary struct {
	kind Kind
	op   Op
	x    Expr
	y    Expr
}

// implement Expr interface
func (e Binary) expr() {}

func (e Binary) RegId() RegId {
	return NoRegId
}

func (e Binary) Kind() Kind {
	return e.kind
}

func (e Binary) Const() bool {
	// access to array element or struct field cannot be a constant
	return e.op != BRACKET && e.op != FIELD && e.x.Const() && e.y.Const()
}

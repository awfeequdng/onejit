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
 * call.go
 *
 *  Created on Dec 23, 2020
 *      Author Massimiliano Ghilardi
 */

package common

// ================================== CallExpr =================================

type CallExpr struct {
	list []Expr
	narg  int
}

func (c CallExpr) Func() Expr {
	return c.list[0]
}

func (c CallExpr) NumIn() int {
	return c.narg
}

func (c CallExpr) NumOut() int {
	return len(c.list) - c.narg - 1
}

func (c CallExpr) In(i int) Expr {
	return c.list[1 : c.narg+1][i]
}

func (c CallExpr) Out(i int) Expr {
	return c.list[c.narg+1:][i]
}

// implement Expr interface
func (c CallExpr) expr() {}

func (c CallExpr) RegId() RegId {
	return NoRegId
}

func (c CallExpr) Kind() Kind {
	if c.NumOut() == 0 {
		return Void
	}
	return c.list[c.narg+1].Kind()
}

func (c CallExpr) IsConst() bool {
	return false
}

func (c CallExpr) Size() Size {
	return c.Kind().Size()
}

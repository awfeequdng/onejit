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
 * unary.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/gomacrojit/jit/internal"
)

func Unary(op Op, x Expr) Expr {
	return internal.Unary(op, x)
}

func Cast(to Kind, x Expr) *UnaryExpr {
	return internal.Cast(to, x)
}

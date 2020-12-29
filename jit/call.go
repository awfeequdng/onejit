/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * call.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/onejit/jit/internal"
)

func Call(fn Expr, sig *Signature, args ...Expr) *CallExpr {
	return internal.Call(fn, sig, args...)
}

// also allows inlining
func CallFunc(fun *Func, args ...Expr) *CallExpr {
	return internal.CallFunc(fun, args...)
}

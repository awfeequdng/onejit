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
 * stmt.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/gomacrojit/jit/internal"
)

func ToStmt(expr Expr) *ExprStmt {
	return internal.ToStmt(expr)
}

func If(cond Expr, then_ Stmt, else_ Stmt) *IfStmt {
	return internal.If(cond, then_, else_)
}

func Block(list ...Stmt) *BlockStmt {
	return internal.Block(list...)
}

func Break() *BreakStmt {
	return internal.Break()
}

func Continue() *ContinueStmt {
	return internal.Continue()
}

func For(init Stmt, cond Expr, post Stmt, body Stmt) *ForStmt {
	return internal.For(init, cond, post, body)
}

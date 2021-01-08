/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * stmt.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/onejit/go/jit/internal"
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

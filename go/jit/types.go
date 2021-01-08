/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
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
 * types.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/onejit/go/jit/internal"
)

type (
	Amd64Mem  = internal.Amd64Mem
	Arch      = internal.Arch
	ArchId    = internal.ArchId
	Assembler = internal.Assembler
	Const     = internal.Const
	CpuWidth  = internal.CpuWidth
	Func      = internal.Func
	Label     = internal.Label
	Labels    = internal.Labels
	Kind      = internal.Kind
	Mem       = internal.Mem
	Op        = internal.Op
	Reg       = internal.Reg
	RegId     = internal.RegId
	Signature = internal.Signature
	Size      = internal.Size

	Expr       = internal.Expr
	BinaryExpr = internal.BinaryExpr
	CallExpr   = internal.CallExpr
	TupleExpr  = internal.TupleExpr
	UnaryExpr  = internal.UnaryExpr

	Stmt         = internal.Stmt
	BlockStmt    = internal.BlockStmt
	BreakStmt    = internal.BreakStmt
	ContinueStmt = internal.ContinueStmt
	ExprStmt     = internal.ExprStmt
	ForStmt      = internal.ForStmt
	IfStmt       = internal.IfStmt

	Source   = internal.Source
	Compiled = internal.Compiled
	Asm      = internal.Asm
)

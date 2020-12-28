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
 * types.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/gomacrojit/jit/internal"
)

type (
	Amd64Mem  = internal.Amd64Mem
	Arch      = internal.Arch
	ArchId    = internal.ArchId
	Assembler = internal.Assembler
	Const     = internal.Const
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

	Source       = internal.Source
	Compiled     = internal.Compiled
	ArchCompiled = internal.ArchCompiled
)

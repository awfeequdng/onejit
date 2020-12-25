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
 * stmt.go
 *
 *  Created on Dec 24, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"fmt"
)

type Stmt interface {
	compile(f *Func)
	fmt.Formatter
	printable
}

// ============================ ExprStmt =========================================

type ExprStmt struct {
	expr Expr
}

func ToStmt(expr Expr) *ExprStmt {
	return &ExprStmt{
		expr: expr,
	}
}

func (s *ExprStmt) Expr() Expr {
	return s.expr
}

// implement Stmt interface
func (s *ExprStmt) compile(f *Func) {
	f.AddCompiled(s.expr)
}

// ============================ IfStmt ===========================================

type IfStmt struct {
	cond  Expr
	then_ Stmt
	else_ Stmt
}

func If(cond Expr, then_ Stmt, else_ Stmt) *IfStmt {
	if kind := cond.Kind(); kind != Bool {
		badOpKind(JUMP_IF, kind)
	}
	return &IfStmt{
		cond:  cond,
		then_: then_,
		else_: else_,
	}
}

func (s *IfStmt) Cond() Expr {
	return s.cond
}

func (s *IfStmt) Then() Stmt {
	return s.then_
}

func (s *IfStmt) Else() Stmt {
	return s.else_
}

// implement Stmt interface
func (s *IfStmt) compile(f *Func) {
	labelEndif := f.NewLabel()
	labelElse := labelEndif
	if s.Else() != nil {
		labelElse = f.NewLabel()
	}
	f.AddCompiled(Binary(JUMP_IF, labelElse, Unary(NOT, s.Cond())))
	s.Then().compile(f)
	if s.Else() != nil {
		f.AddCompiled(Unary(JUMP, labelEndif))
		f.AddCompiled(labelElse)
		s.Else().compile(f)
	}
	f.AddCompiled(labelEndif)
}

// ============================ BlockStmt ========================================

type BlockStmt struct {
	list []Stmt
}

func (s *BlockStmt) Len() int {
	return len(s.list)
}

func (s *BlockStmt) At(i int) Stmt {
	return s.list[i]
}

func Block(list ...Stmt) *BlockStmt {
	return &BlockStmt{
		list: list,
	}
}

func BlockSlice(list []Stmt) *BlockStmt {
	return &BlockStmt{
		list: append([]Stmt(nil), list...),
	}
}

func (s *BlockStmt) compile(f *Func) {
	for _, stmt := range s.list {
		stmt.compile(f)
	}
}

// ============================ BreakStmt ======================================

type BreakStmt struct{}

var breakStmt = &BreakStmt{}

func Break() *BreakStmt {
	return breakStmt
}

func (s *BreakStmt) compile(f *Func) {
	f.AddCompiled(Unary(JUMP, f.Breaks().Top()))
}

// ============================ ContinueStmt ===================================

type ContinueStmt struct{}

var continueStmt = &ContinueStmt{}

func Continue() *ContinueStmt {
	return continueStmt
}

func (s *ContinueStmt) compile(f *Func) {
	f.AddCompiled(Unary(JUMP, f.Continues().Top()))
}

// ============================ ForStmt ========================================

type ForStmt struct {
	init Stmt
	cond Expr
	post Stmt
	body Stmt
}

func (s *ForStmt) Init() Stmt {
	return s.init
}

func (s *ForStmt) Cond() Expr {
	return s.cond
}

func (s *ForStmt) Post() Stmt {
	return s.post
}

func (s *ForStmt) Body() Stmt {
	return s.body
}

func For(init Stmt, cond Expr, post Stmt, body Stmt) *ForStmt {
	if cond != nil && cond.Kind() != Bool {
		badOpKind(JUMP_IF, cond.Kind())
	}
	return &ForStmt{
		init: init,
		cond: cond,
		post: post,
		body: body,
	}
}

func (s *ForStmt) compile(f *Func) {
	labelLoop := f.NewLabel()
	labelTest := f.NewLabel()
	labelBreak := f.NewLabel()

	f.Breaks().Push(labelBreak)
	f.Continues().Push(labelTest)
	defer func() {
		f.Breaks().Pop()
		f.Continues().Pop()
	}()

	if s.Init() != nil {
		s.Init().compile(f)
	}
	if s.Cond() != nil {
		f.AddCompiled(Unary(JUMP, labelTest))
	}
	f.AddCompiled(labelLoop)
	s.Body().compile(f)
	if s.Post() != nil {
		s.Post().compile(f)
	}
	f.AddCompiled(labelTest)
	if s.Cond() != nil {
		f.AddCompiled(Binary(JUMP_IF, labelLoop, s.Cond()))
	} else {
		f.AddCompiled(Unary(JUMP_IF, labelLoop))
	}
	f.AddCompiled(labelBreak)
}

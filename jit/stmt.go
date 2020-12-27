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
 *  Created on Dec 24, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"fmt"
)

type Stmt interface {
	Node
	compileTo(to *Compiled)
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
func (s *ExprStmt) Children() int {
	return 1
}

func (s *ExprStmt) Child(i int) Node {
	if i == 0 {
		return s.expr
	}
	return badIndex(i, 0)
}

func (s *ExprStmt) compileTo(c *Compiled) {
	c.Add(s.expr)
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
func (s *IfStmt) Children() int {
	return 3
}

func (s *IfStmt) Child(i int) Node {
	switch i {
	case 0:
		return s.Cond()
	case 1:
		return s.Then()
	case 2:
		return s.Else()
	default:
		return badIndex(i, 3)
	}
}

func (s *IfStmt) compileTo(c *Compiled) {
	f := c.Func()
	labelEndif := f.NewLabel()
	labelElse := labelEndif
	if s.Else() != nil {
		labelElse = f.NewLabel()
	}
	c.Add(Binary(JUMP_IF, labelElse, Unary(LNOT, s.Cond())))
	s.Then().compileTo(c)
	if s.Else() != nil {
		c.Add(Unary(JUMP, labelEndif))
		c.Add(labelElse)
		s.Else().compileTo(c)
	}
	c.Add(labelEndif)
}

// ============================ BlockStmt ========================================

type BlockStmt struct {
	list []Stmt
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

// implement Stmt interface
func (s *BlockStmt) Children() int {
	return len(s.list)
}

func (s *BlockStmt) Child(i int) Node {
	return s.list[i]
}

func (s *BlockStmt) compileTo(c *Compiled) {
	for _, stmt := range s.list {
		stmt.compileTo(c)
	}
}

// ============================ BreakStmt ======================================

type BreakStmt struct{}

var breakStmt = &BreakStmt{}

func Break() *BreakStmt {
	return breakStmt
}

// implement Stmt interface
func (s *BreakStmt) Children() int {
	return 0
}

func (s *BreakStmt) Child(i int) Node {
	return badIndex(i, 0)
}

func (s *BreakStmt) compileTo(c *Compiled) {
	c.Add(Unary(JUMP, c.Func().Breaks().Top()))
}

// ============================ ContinueStmt ===================================

type ContinueStmt struct{}

var continueStmt = &ContinueStmt{}

func Continue() *ContinueStmt {
	return continueStmt
}

// implement Stmt interface
func (s *ContinueStmt) Children() int {
	return 0
}

func (s *ContinueStmt) Child(i int) Node {
	return badIndex(i, 0)
}

func (s *ContinueStmt) compileTo(c *Compiled) {
	c.Add(Unary(JUMP, c.Func().Continues().Top()))
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

// implement Stmt interface
func (s *ForStmt) Children() int {
	return 4
}

func (s *ForStmt) Child(i int) Node {
	switch i {
	case 0:
		return s.init
	case 1:
		return s.cond
	case 2:
		return s.post
	case 3:
		return s.body
	default:
		return badIndex(i, 4)
	}
}

func (s *ForStmt) compileTo(c *Compiled) {
	f := c.Func()
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
		s.Init().compileTo(c)
	}
	if s.Cond() != nil {
		c.Add(Unary(JUMP, labelTest))
	}
	c.Add(labelLoop)
	{
		s.Body().compileTo(c)
	}
	if s.Post() != nil {
		s.Post().compileTo(c)
	}
	c.Add(labelTest)
	if s.Cond() != nil {
		c.Add(Binary(JUMP_IF, labelLoop, s.Cond()))
	} else {
		c.Add(Unary(JUMP_IF, labelLoop))
	}
	c.Add(labelBreak)
}

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
 * function.go
 *
 *  Created on Dec 23, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

// ================================== Signature ================================

type Signature struct {
	kinds  []Kind
	nparam int
}

func NewSignature(params []Kind, results []Kind) *Signature {
	// deep copy slices
	nparam := len(params)
	kinds := make([]Kind, nparam+len(results))
	copy(kinds, params)
	copy(kinds[nparam:], results)

	return &Signature{
		kinds:  kinds,
		nparam: nparam,
	}
}

func (s *Signature) NumIn() int {
	return s.nparam
}

func (s *Signature) NumOut() int {
	return len(s.kinds) - s.nparam
}

func (s *Signature) In(i int) Kind {
	return s.kinds[:s.nparam][i]
}

func (s *Signature) Out(i int) Kind {
	return s.kinds[s.nparam+i]
}

// ================================== Label ====================================

type Label struct {
	// allocated on creation, *addr computed lazily
	addr  *uintptr
	index int // only for debugging
}

// implement Expr interface
func (m Label) expr() {}

func (m Label) RegId() RegId {
	return NoRegId
}

func (m Label) Kind() Kind {
	return Ptr
}

func (m Label) IsConst() bool {
	// Label is a link-time constant
	return true
}

func (m Label) Size() Size {
	return 0
}

// ================================== Func =====================================

type Func struct {
	name       string
	sig        *Signature
	regs       []Reg
	labels     []Label
	labelPool  []uintptr
	labelIndex int
	code       []Expr
}

func NewFunc(name string, sig *Signature) *Func {
	f := &Func{
		name: name,
		sig:  sig,
	}
	f.NewLabel() // prepare f.Label()
	narg := sig.NumIn()
	for i := 0; i < narg; i++ {
		f.NewReg(sig.In(i))
	}
	return f
}

func (f *Func) Name() string {
	return f.name
}

func (f *Func) Signature() *Signature {
	return f.sig
}

// return number of arguments
func (f *Func) NumArg() int {
	return f.sig.NumIn()
}

// return number of results
func (f *Func) NumRet() int {
	return f.sig.NumOut()
}

// return i-th argument
func (f *Func) Arg(i int) Reg {
	return f.regs[:f.NumArg()][i]
}

/* return a label pointing to function's beginning */
func (f *Func) Label() Label {
	return f.labels[0]
}

func (f *Func) AddExpr(e Expr) *Func {
	f.code = append(f.code, e)
	return f
}

func (f *Func) AddExprs(e ...Expr) *Func {
	f.code = append(f.code, e...)
	return f
}

func (f *Func) AddStmt(stmt Stmt) *Func {
	switch stmt := stmt.(type) {
	case ExprStmt:
		f.AddExpr(stmt.Expr())
	case IfStmt:
		labelElse := f.NewLabel()
		labelEndif := f.NewLabel()
		f.AddExpr(Binary(JUMP_IF, labelElse, Unary(NOT, stmt.Cond())))
		f.AddStmt(stmt.Then())
		f.AddExpr(Unary(JUMP, labelEndif))
		f.AddExpr(labelElse)
		f.AddStmt(stmt.Else())
		f.AddExpr(labelEndif)
	case BlockStmt:
		for _, s := range stmt.list {
			f.AddStmt(s)
		}
	case WhileStmt:
		labelLoop := f.NewLabel()
		labelTest := f.NewLabel()
		f.AddExpr(Unary(JUMP, labelTest))
		f.AddExpr(labelLoop)
		f.AddStmt(stmt.Body())
		f.AddExpr(labelTest)
		f.AddExpr(Binary(JUMP_IF, labelLoop, stmt.Cond()))
	}
	return f
}

func (f *Func) NewReg(kind Kind) Reg {
	r := Reg{
		kind: kind,
		id:   minSoftRegId + RegId(len(f.regs)),
	}
	f.regs = append(f.regs, r)
	return r
}

func (f *Func) NewLabel() Label {
	if len(f.labelPool) >= cap(f.labelPool) {
		f.labelPool = make([]uintptr, 0, 16)
	}
	f.labelPool = append(f.labelPool, 0) // does not reallocate
	l := Label{
		addr:  &f.labelPool[len(f.labelPool)-1],
		index: f.labelIndex,
	}
	f.labels = append(f.labels, l)
	f.labelIndex++
	return l
}

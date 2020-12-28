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
 * func.go
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

// save Label to a temporary register
func (l Label) spillToReg(ac *ArchCompiled) Reg {
	reg := ac.Func().NewReg(Ptr)
	ac.Add(Binary(ASSIGN, reg, l))
	return reg
}

// implement Expr interface
func (l Label) Class() Class {
	return LABEL
}

func (l Label) Op() Op {
	return CONST
}

func (l Label) Kind() Kind {
	return Ptr
}

func (l Label) expr() {}

func (l Label) RegId() RegId {
	return NoRegId
}

func (l Label) Size() Size {
	return Ptr.Size()
}

func (l Label) Children() int {
	return 0
}

func (l Label) Child(i int) Node {
	return badIndex(i, 0)
}

func (l Label) IsConst() bool {
	// Label is a link-time constant
	return true
}

func (l Label) IsPure() bool {
	// Label is a jump target, cannot be optimized away
	return false
}

// ================================== Labels ===================================

type Labels struct {
	list []Label
}

func (ls *Labels) Top() Label {
	return ls.list[len(ls.list)-1]
}

func (ls *Labels) Push(l Label) {
	ls.list = append(ls.list, l)
}

func (ls *Labels) Pop() {
	ls.list = ls.list[0 : len(ls.list)-1]
}

// ================================== Source ===================================

type Source struct {
	regn   int
	labeln int
	list   []Stmt
}

func (s *Source) AddStmt(stmt Stmt) *Source {
	s.list = append(s.list, stmt)
	return s
}

func (s *Source) AddExpr(e Expr) *Source {
	return s.AddStmt(ToStmt(e))
}

func (s *Source) Children() int {
	return len(s.list)
}

func (s *Source) Child(i int) Node {
	return s.list[i]
}

// ================================== Func =====================================

type Func struct {
	name       string
	sig        *Signature
	regs       []Reg
	labels     []Label
	labelPool  []uintptr
	breaks     Labels
	continues  Labels
	labelIndex int
	source     Source
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

/* return the stack of destination labels for 'break' */
func (f *Func) Breaks() *Labels {
	return &f.breaks
}

/* return the stack of destination labels for 'continue' */
func (f *Func) Continues() *Labels {
	return &f.continues
}

func (f *Func) AddExpr(e Expr) *Func {
	f.source.AddExpr(e)
	f.source.labeln = len(f.labels)
	f.source.regn = len(f.regs)
	return f
}

func (f *Func) AddStmt(s Stmt) *Func {
	f.source.AddStmt(s)
	f.source.labeln = len(f.labels)
	f.source.regn = len(f.regs)
	return f
}

func (f *Func) Add(xs ...interface{}) *Func {
	for _, x := range xs {
		switch x := x.(type) {
		case Expr:
			f.AddExpr(x)
		case Stmt:
			f.AddStmt(x)
		default:
			Errorf("unsupported type: %T", x)
		}
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

func (f *Func) Compile() *Compiled {
	compiled := Compiled{fun: f}
	for _, stmt := range f.source.list {
		stmt.compileTo(&compiled)
	}
	compiled.labeln = len(f.labels)
	compiled.regn = len(f.regs)
	return &compiled
}

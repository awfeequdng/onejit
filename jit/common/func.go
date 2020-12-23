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

package common

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
	return s.kinds[s.nparam:][i]
}

// ================================== Label ====================================

type Label struct {
	// allocated on creation, *addr computed lazily
	addr *uintptr
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
	name      string
	sig       *Signature
	regs      []Reg
	labels    []Label
	labelPool []uintptr
	code      []Expr
}

func NewFunc(name string, sig *Signature) *Func {
	f := &Func{
		name: name,
		sig:  sig,
	}
	narg := sig.NumIn()
	for i := 0; i < narg; i++ {
		f.NewReg(sig.In(i))
	}
	f.NewLabel() // prepare f.Label()
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

func (f *Func) Stmt(e Expr) *Func {
	f.code = append(f.code, e)
	return f
}

func (f *Func) Block(e ...Expr) *Func {
	f.code = append(f.code, e...)
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
		addr: &f.labelPool[len(f.labelPool)-1],
	}
	f.labels = append(f.labels, l)
	return l
}

func (f *Func) NewCall(callf Expr, sig *Signature, args []Expr) CallExpr {
	narg := len(args)
	if n := sig.NumIn(); narg != n {
		Errorf("bad number of arguments in call: have %d, want %d", narg, n)
	}
	for i := range args {
		karg := args[i].Kind()
		kind := sig.In(i)
		if karg != kind {
			Errorf("bad argument %v in call: have %d, want %d", i+1, karg, kind)
		}
	}
	nret := sig.NumOut()

	exprs := make([]Expr, 1+narg+nret)
	{
		exprs[0] = callf
		copy(exprs[1:], args)
		for i := 0; i < nret; i++ {
			exprs[1+narg+i] = f.NewReg(sig.Out(i))
		}
	}
	return CallExpr{
		exprs: exprs,
		narg:  narg,
	}
}

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

func (m Label) Const() bool {
	// Label is a link-time constant
	return true
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
	return &Func{
		name: name,
		sig:  sig,
	}
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
	_ = append(f.labelPool, 0) // does not reallocate
	return Label{
		addr: &f.labelPool[len(f.labelPool)-1],
	}
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

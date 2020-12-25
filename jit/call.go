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
 * call.go
 *
 *  Created on Dec 23, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

// ================================== *CallExpr =================================

type CallExpr struct {
	sig  *Signature
	fun  *Func // nil if not known
	list []Expr
}

func Call(fn Expr, sig *Signature, args []Expr) *CallExpr {
	if kind := fn.Kind(); kind != Ptr {
		Errorf("bad function kind in call: have %v, want %v", kind, Ptr)
	}
	if sig == nil {
		Errorf("nil signature in call")
	}
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
	list := make([]Expr, len(args)+1)
	list[0] = fn
	copy(list[1:], args)
	return &CallExpr{
		sig:  sig,
		list: list,
	}
}

// also allows inlining
func CallFunc(fun *Func, args []Expr) *CallExpr {
	call := Call(fun.Label(), fun.Signature(), args)
	call.fun = fun
	return call
}

// return nil if not known
func (c *CallExpr) Func() *Func {
	return c.fun
}

func (c *CallExpr) FuncExpr() Expr {
	return c.list[0]
}

func (c *CallExpr) NumArg() int {
	return len(c.list) - 1
}

func (c *CallExpr) Arg(i int) Expr {
	return c.list[i+1]
}

func (c *CallExpr) Signature() *Signature {
	return c.sig
}

// implement Expr interface
func (c *CallExpr) expr() {}

func (c *CallExpr) RegId() RegId {
	return NoRegId
}

func (c *CallExpr) Kind() Kind {
	if c.sig.NumOut() == 0 {
		return Void
	}
	return c.sig.Out(0)
}

func (c *CallExpr) IsConst() bool {
	return false
}

func (c *CallExpr) Size() Size {
	return c.Kind().Size()
}

func (c *CallExpr) Children() int {
	return len(c.list)
}

func (c *CallExpr) Child(i int) Expr {
	return c.list[i]
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * code.go
 *
 *  Created on: Apr 23, 2021
 *      Author: Massimiliano Ghilardi
 */

// some random code, used for testing package go/typecheck

package testdata

const (
	Iota0 = iota
	Iota1

	Foo int = Bar >> 70
	Bar     = '\x01' << 100
)

// var Len = len((*Pair)(nil).first)

type (
	pair = struct {
		first, second int
	}
	Pair pair

	X *X

	foobar = interface {
		foo() (result Pair)
		bar(arg Pair)
	}
	FooBar foobar
)

type (
	RecurA struct{ *RecurB }
	RecurB RecurA
)

type (
	Env  struct{}
	Stmt func(*Env) (*Env, Stmt)
)

var (
	three = 3
	env   *Env
	stmt  Stmt
)

func run(env *Env, stmt Stmt) (*Env, Stmt) {
	for env != nil && stmt != nil {
		env, stmt = stmt(env)
	}
	return env, stmt
}

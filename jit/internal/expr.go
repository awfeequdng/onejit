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
 * expr.go
 *
 *  Created on Dec 20, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

import (
	"fmt"
)

type Class uint8

// Class values are ordered from strongest "keep at left in binary expressions"
// to strongest "keep at right in binary expressions"
// this simplifies code that pattern-matches expression trees
const (
	REG Class = iota
	MEM
	UNARY
	BINARY
	TUPLE
	CALL
	LABEL
	CONSTANT
	STMT
)

type Node interface {
	Class() Class
	Op() Op
	Kind() Kind
	Children() int
	Child(i int) Node
}

type Expr interface {
	expr() // private marker
	RegId() RegId
	IsConst() bool
	// true if expression only computes a result, without any assignment, jump or call
	IsPure() bool
	Size() Size
	Node
	fmt.Formatter
	fmt.Stringer
}

type noteq [0]func() // not comparable

func ExprMustBeAssignable(op Op, x Expr) Kind {
	switch x := x.(type) {
	case Reg:
		if x.IsAssignable() {
			return Void
		}
	case Mem:
		if x.IsAssignable() {
			return Void
		}
	}
	Errorf("cannot assign to expression: %v %T", op, x)
	return Void
}

func IsZero(e Node) bool {
	c, ok := e.(Const)
	return ok && c.re == 0 && c.im == 0
}

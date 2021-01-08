/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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

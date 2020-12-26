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

package jit

import (
	"fmt"
)

type Node interface {
	Children() int
	Child(i int) Node
}

type Expr interface {
	RegId() RegId
	Kind() Kind
	IsConst() bool
	Size() Size
	Node
	expr() // private marker
	fmt.Formatter
}

type noteq [0]func() // not comparable

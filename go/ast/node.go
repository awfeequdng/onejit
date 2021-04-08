/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * node.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

type Node interface {
	Op() token.Token
	Len() int      // return number of children
	At(i int) Node // get i-th child
	Pos() token.Pos
	End() token.Pos
	Comments() []string
	String() string
}

var strNil = []byte("nil")

func NodeString(node Node) string {
	if node == nil {
		return "nil"
	}
	return node.String()
}

func choose2(i int, a Node, b Node) (ret Node) {
	switch i {
	case 0:
		ret = a
	case 1:
		ret = b
	default:
		ret = outOfRange()
	}
	return ret
}

func choose3(i int, a Node, b Node, c Node) (ret Node) {
	switch i {
	case 0:
		ret = a
	case 1:
		ret = b
	case 2:
		ret = c
	default:
		ret = outOfRange()
	}
	return ret
}

func choose4(i int, a Node, b Node, c Node, d Node) (ret Node) {
	switch i {
	case 0:
		ret = a
	case 1:
		ret = b
	case 2:
		ret = c
	case 3:
		ret = d
	default:
		ret = outOfRange()
	}
	return ret
}

func outOfRange() Node {
	return *(*Node)(nil)
}

func formatAsList(f fmt.State, verb rune, list Node) {
	fmt.Fprintf(f, "(%v", list.Op())
	for i, n := 0, list.Len(); i < n; i++ {
		node := list.At(i)
		if formatter, ok := node.(fmt.Formatter); ok {
			f.Write([]byte{' '})
			formatter.Format(f, verb)
		} else if node == nil {
			f.Write([]byte(" nil"))
		} else {
			fmt.Fprintf(f, " %v", node)
		}
	}
	f.Write([]byte{')'})
}

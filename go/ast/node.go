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
	"github.com/cosmos72/onejit/go/io"
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
	WriteTo(out io.StringWriter)
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

// always panics
func outOfRange() Node {
	return ([]Node)(nil)[0]
}

func writeListTo(out io.StringWriter, list Node) {
	out.WriteString("(")
	out.WriteString(list.Op().String())
	for i, n := 0, list.Len(); i < n; i++ {
		out.WriteString(" ")
		if node := list.At(i); node == nil {
			out.WriteString("nil")
		} else {
			node.WriteTo(out)
		}
	}
	out.WriteString(")")
}

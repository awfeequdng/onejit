/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * bad.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

type Bad struct {
	Atom // contains expected token
	Node Node
	Err  error
}

func (b *Bad) Len() int {
	return 1
}

func (b *Bad) At(i int) Node {
	if i == 0 {
		return b.Node
	}
	return outOfRange()
}

func (b *Bad) End() token.Pos {
	if b.Node != nil {
		return b.Node.End()
	} else {
		return b.Atom.End()
	}
}

func (b *Bad) String() string {
	if b == nil {
		return "Bad"
	} else {
		return fmt.Sprint(b)
	}
}

func (b *Bad) Format(out fmt.State, verb rune) {
	if b == nil {
		out.Write([]byte("Bad"))
	} else if b.Node == nil {
		if len(b.Lit) == 0 {
			fmt.Fprintf(out, "(Bad %v)", b.Tok)
		} else {
			fmt.Fprintf(out, "(Bad %v %v)", b.Tok, b.Lit)
		}
	} else if b.Tok == b.Node.Op() {
		fmt.Fprintf(out, "(Bad %v)", b.Node)
	} else {
		fmt.Fprintf(out, "(Bad %v %v)", b.Tok, b.Node)
	}
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * atom.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

type Atom struct {
	Tok     token.Token
	Lit     string
	TokPos  token.Pos
	TokEnd  token.Pos
	Comment []string
}

func (a *Atom) Op() token.Token {
	return a.Tok
}

func (a *Atom) Len() int {
	return 0
}

func (a *Atom) At(_ int) Node {
	return outOfRange()
}

func (a *Atom) Pos() token.Pos {
	return a.TokPos
}

func (a *Atom) End() token.Pos {
	return a.TokEnd
}

func (a *Atom) Comments() []string {
	return a.Comment
}

func (a *Atom) String() string {
	if a == nil {
		return "nil"
	} else {
		return fmt.Sprint(a)
	}
}

func (a *Atom) Format(out fmt.State, verb rune) {
	if a == nil {
		out.Write(strNil)
	} else if len(a.Lit) != 0 {
		fmt.Fprintf(out, "(%v %q)", a.Tok, a.Lit)
	} else {
		fmt.Fprintf(out, "(%v)", a.Tok)
	}
}

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
	// different order than Atom,
	// to avoid explicit conversion between the two
	Tok    token.Token
	TokPos token.Pos
	TokEnd token.Pos
	Lit    string
}

func (b *Bad) Op() token.Token {
	return b.Tok
}

func (b *Bad) Size() int {
	return 0
}

func (b *Bad) Get(_ int) Node {
	return ([]Node)(nil)[0]
}

func (b *Bad) Pos() token.Pos {
	return b.TokPos
}

func (b *Bad) End() token.Pos {
	return b.TokEnd
}

func (b *Bad) String() string {
	return fmt.Sprintf("%v", b)
}

func (b *Bad) Format(f fmt.State, verb rune) {
	if len(b.Lit) != 0 {
		fmt.Fprintf(f, "(Bad %v %q)", b.Tok, b.Lit)
	} else {
		fmt.Fprintf(f, "(Bad %v)", b.Tok)
	}
}

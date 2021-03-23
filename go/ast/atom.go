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

import "github.com/cosmos72/onejit/go/token"

type Atom struct {
	Tok    token.Token
	Lit    string
	TokPos token.Pos
	TokEnd token.Pos
}

func (a *Atom) Op() token.Token {
	return a.Tok
}

func (a *Atom) Size() int {
	return 0
}

func (a *Atom) Get(_ int) Node {
	return ([]Node)(nil)[0]
}

func (a *Atom) Pos() token.Pos {
	return a.TokPos
}

func (a *Atom) End() token.Pos {
	return a.TokEnd
}

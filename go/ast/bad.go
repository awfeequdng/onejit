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

import "github.com/cosmos72/onejit/go/token"

type Bad struct {
	Tok    token.Token
	BadPos token.Pos
	BadEnd token.Pos
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
	return b.BadPos
}

func (b *Bad) End() token.Pos {
	return b.BadEnd
}

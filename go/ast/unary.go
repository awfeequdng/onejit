/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * unary.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

type Unary struct {
	Atom
	X Node
}

func (u *Unary) Len() int {
	return 1
}

func (u *Unary) At(i int) Node {
	if i == 0 {
		return u.X
	}
	return outOfRange()
}

func (u *Unary) End() token.Pos {
	if u.X != nil {
		return u.X.End()
	} else {
		return u.Atom.End()
	}
}

func (u *Unary) String() string {
	if u == nil {
		return "nil"
	} else {
		return fmt.Sprint(u)
	}
}

func (u *Unary) Format(out fmt.State, verb rune) {
	if u == nil {
		out.Write(strNil)
	} else {
		formatAsList(out, verb, u)
	}
}

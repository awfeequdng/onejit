/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * binary.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

type Binary struct {
	Atom
	X Node
	Y Node
}

func (b *Binary) Len() int {
	return 2
}

func (b *Binary) At(i int) Node {
	switch i {
	case 0:
		return b.X
	case 1:
		return b.Y
	default:
		return outOfRange()
	}
}

func (b *Binary) End() token.Pos {
	if b.Y != nil {
		return b.Y.End()
	} else if b.X != nil {
		return b.X.End()
	} else {
		return b.Atom.End()
	}
}

func (b *Binary) String() string {
	if b == nil {
		return "nil"
	} else {
		return fmt.Sprint(b)
	}
}

func (b *Binary) Format(out fmt.State, verb rune) {
	if b == nil {
		out.Write(strNil)
	} else {
		formatAsList(out, verb, b)
	}
}

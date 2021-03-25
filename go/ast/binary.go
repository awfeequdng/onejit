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

func (b *Binary) At(i int) (ret Node) {
	switch i {
	case 0:
		ret = b.X
	case 1:
		ret = b.Y
	default:
		ret = outOfRange()
	}
	return ret
}

func (b *Binary) End() (pos token.Pos) {
	if b.Y != nil {
		pos = b.Y.End()
	} else if b.X != nil {
		pos = b.X.End()
	} else {
		pos = b.Atom.End()
	}
	return pos
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

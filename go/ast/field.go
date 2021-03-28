/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * field.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

type Field struct {
	Atom
	Names *List
	Type  Node
	Tag   *Atom
}

func (f *Field) Len() int {
	if f.Tag == nil {
		return 2
	}
	return 3
}

func (f *Field) At(i int) Node {
	if i == 2 && f.Tag == nil {
		return outOfRange()
	}
	return choose3(i, f.Names, f.Type, f.Tag)
}

func (f *Field) End() token.Pos {
	if f.Tag != nil {
		return f.Tag.End()
	} else if f.Type != nil {
		return f.Type.End()
	} else if f.Names != nil {
		return f.Names.End()
	} else {
		return f.Atom.End()
	}
}

func (f *Field) String() string {
	if f == nil {
		return "nil"
	} else {
		return fmt.Sprint(f)
	}
}

func (f *Field) Format(out fmt.State, verb rune) {
	if f == nil {
		out.Write(strNil)
	} else {
		formatAsList(out, verb, f)
	}
}

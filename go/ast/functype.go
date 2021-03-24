/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, f. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * functype.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

type FuncType struct {
	Atom
	Params  *List
	Results *List
}

func (f *FuncType) Len() int {
	return 2
}

func (f *FuncType) At(i int) Node {
	return choose2(i, f.Params, f.Results)
}

func (f *FuncType) End() token.Pos {
	if f.Results != nil {
		return f.Results.End()
	} else if f.Params != nil {
		return f.Params.End()
	} else {
		return f.Atom.End()
	}
}

func (f *FuncType) String() string {
	if f == nil {
		return "nil"
	} else {
		return fmt.Sprint(f)
	}
}

func (f *FuncType) Format(out fmt.State, verb rune) {
	if f == nil {
		out.Write(strNil)
	} else {
		formatAsList(out, verb, f)
	}
}

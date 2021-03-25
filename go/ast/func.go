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
	Atom    // always token.FUNC
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

////////////////////////////////////////////////////////////////////////////////

type FuncDecl struct {
	Atom       // always token.FUNC
	Recv *List // nil for functions, non-nil for methods
	Name Node
	Type *FuncType
	Body *List
}

func (f *FuncDecl) Len() int {
	return 4
}

func (f *FuncDecl) At(i int) Node {
	return choose4(i, f.Recv, f.Name, f.Type, f.Body)
}

func (f *FuncDecl) End() token.Pos {
	if f.Body != nil {
		return f.Body.End()
	} else if f.Type != nil {
		return f.Type.End()
	} else if f.Name != nil {
		return f.Name.End()
	} else if f.Recv != nil {
		return f.Recv.End()
	} else {
		return f.Atom.End()
	}
}

func (f *FuncDecl) String() string {
	if f == nil {
		return "nil"
	} else {
		return fmt.Sprint(f)
	}
}

func (f *FuncDecl) Format(out fmt.State, verb rune) {
	if f == nil {
		out.Write(strNil)
	} else {
		formatAsList(out, verb, f)
	}
}

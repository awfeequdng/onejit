/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * func.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/strings"
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

func (f *FuncType) At(i int) (child Node) {
	// cannot use choose2() here: assigning a nil pointer to an interface
	// creates a "half-nil" interface
	switch i {
	case 0:
		if node := f.Params; node != nil {
			child = node
		}
	case 1:
		if node := f.Results; node != nil {
			child = node
		}
	default:
		outOfRange()
	}
	return child
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
	}
	var buf strings.Builder
	f.WriteTo(&buf)
	return buf.String()
}

func (f *FuncType) WriteTo(out io.StringWriter) {
	if f == nil {
		out.WriteString("nil")
	} else {
		writeListTo(out, f)
	}
}

////////////////////////////////////////////////////////////////////////////////

type FuncDecl struct {
	Atom       // always token.FUNC
	Recv *List // nil for functions, non-nil for methods
	Name Node
	Type Node // *ast.FunctType or *ast.GenericType
	Body *List
}

func (f *FuncDecl) Len() int {
	return 4
}

func (f *FuncDecl) At(i int) (child Node) {
	// cannot use choose4() here: assigning a nil pointer to an interface
	// creates a "half-nil" interface
	switch i {
	case 0:
		if node := f.Recv; node != nil {
			child = node
		}
	case 1:
		child = f.Name
	case 2:
		child = f.Type
	case 3:
		if node := f.Body; node != nil {
			child = node
		}
	default:
		outOfRange()
	}
	return child
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
	}
	var buf strings.Builder
	f.WriteTo(&buf)
	return buf.String()
}

func (f *FuncDecl) WriteTo(out io.StringWriter) {
	if f == nil {
		out.WriteString("nil")
	} else {
		writeListTo(out, f)
	}
}

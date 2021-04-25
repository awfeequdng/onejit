/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * file.go
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

type File struct {
	Atom
	Package *Unary
	Imports *List
	Decls   *List
	File    *token.File
}

func (f *File) Len() int {
	return 3
}

func (f *File) At(i int) (child Node) {
	// cannot use choose3() here: assigning a nil pointer to an interface
	// creates a "half-nil" interface
	switch i {
	case 0:
		if node := f.Package; node != nil {
			child = node
		}
	case 1:
		if node := f.Imports; node != nil {
			child = node
		}
	case 2:
		if node := f.Decls; node != nil {
			child = node
		}
	default:
		outOfRange()
	}
	return child
}

func (f *File) End() token.Pos {
	if f.Decls != nil {
		return f.Decls.End()
	} else if f.Imports != nil {
		return f.Imports.End()
	} else if f.Package != nil {
		return f.Package.End()
	} else {
		return f.Atom.End()
	}
}

func (f *File) PkgName() string {
	if f.Package == nil {
		return ""
	} else if ident, _ := f.Package.X.(*Atom); ident == nil {
		return ""
	} else {
		return ident.Lit
	}
}

func (f *File) String() string {
	if f == nil {
		return "nil"
	}
	var buf strings.Builder
	f.WriteTo(&buf)
	return buf.String()
}

func (f *File) WriteTo(out io.StringWriter) {
	if f == nil {
		out.WriteString("nil")
	} else {
		writeListTo(out, f)
	}
}

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
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

type File struct {
	Atom
	Package *Unary
	Imports *List
	Decls   *List
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

func (f *File) String() string {
	if f == nil {
		return "nil"
	} else {
		return fmt.Sprint(f)
	}
}

func (f *File) Format(out fmt.State, verb rune) {
	if f == nil {
		out.Write(strNil)
	} else {
		formatAsList(out, verb, f)
	}
}

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

type Dir struct {
	Atom
	Files   []*File
	FileSet *token.FileSet
}

func (d *Dir) Len() int {
	return len(d.Files)
}

func (d *Dir) At(i int) (child Node) {
	// cannot use child = d.Files[i] here: assigning a nil pointer to an interface
	// creates a "half-nil" interface
	if file := d.Files[i]; file != nil {
		child = file
	}
	return child
}

func (d *Dir) End() token.Pos {
	if n := len(d.Files); n != 0 {
		return d.Files[n-1].End()
	} else {
		return d.Atom.End()
	}
}

// return package name
func (d *Dir) PkgName() string {
	if len(d.Files) == 0 {
		return strings.Basename(d.PkgPath())
	}
	return d.Files[0].PkgName()
}

// return package path
func (d *Dir) PkgPath() string {
	if d == nil {
		return ""
	}
	return d.FileSet.PkgPath()
}

func (d *Dir) String() string {
	if d == nil {
		return "nil"
	}
	var buf strings.Builder
	d.WriteTo(&buf)
	return buf.String()
}

func (d *Dir) WriteTo(out io.StringWriter) {
	if d == nil {
		out.WriteString("nil")
	} else {
		writeListTo(out, d)
	}
}

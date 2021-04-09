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
	Files []*File
}

func (d *Dir) Len() int {
	return len(d.Files)
}

func (d *Dir) At(i int) (child Node) {
	return d.Files[i]
}

func (d *Dir) End() token.Pos {
	if n := len(d.Files); n != 0 {
		return d.Files[n-1].End()
	} else {
		return d.Atom.End()
	}
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

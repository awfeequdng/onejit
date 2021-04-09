/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * bad.go
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

type Bad struct {
	Atom // contains expected token
	Node Node
	Err  error
}

func (b *Bad) Len() int {
	return 1
}

func (b *Bad) At(i int) Node {
	if i == 0 {
		return b.Node
	}
	return outOfRange()
}

func (b *Bad) End() token.Pos {
	if b.Node != nil {
		return b.Node.End()
	} else {
		return b.Atom.End()
	}
}

func (b *Bad) String() string {
	if b == nil {
		return "nil"
	}
	var buf strings.Builder
	b.WriteTo(&buf)
	return buf.String()
}

func (b *Bad) WriteTo(out io.StringWriter) {
	if b == nil {
		out.WriteString("Bad")
		return
	}
	out.WriteString("(Bad ")
	if b.Node != nil {
		b.Node.WriteTo(out)
	} else {
		out.WriteString(b.Tok.String())
		if len(b.Lit) != 0 {
			out.WriteString(" ")
			out.WriteString(b.Lit)
		}
	}
	out.WriteString(")")
}

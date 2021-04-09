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
	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/token"
)

/**
 * Node with two children. Used for binary expressions, closures, types,
 * RANGE, SEND and other two-argument statements.
 *
 * LAMBDA content is: functype funcbody
 * RANGE  content is: init block
 * SEND   content is: channel expr
 */
type Binary struct {
	Atom
	X Node
	Y Node
}

func (b *Binary) Len() int {
	return 2
}

func (b *Binary) At(i int) (ret Node) {
	return choose2(i, b.X, b.Y)
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
	}
	var buf strings.Builder
	b.WriteTo(&buf)
	return buf.String()
}

func (b *Binary) WriteTo(out io.StringWriter) {
	if b == nil {
		out.WriteString("nil")
	} else {
		writeListTo(out, b)
	}
}

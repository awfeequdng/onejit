/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * list.go
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
 * List of nodes. Used for all variable-sized array of nodes,
 * and also for many fixed-size array of nodes, including:
 * BLOCK, CALL, COMPOSITE_LIT, FOR, IF, INDEX, RETURN, SELECT, SWITCH.
 *
 * BLOCK  content is: [stmt0 [stmt1 [...]]
 * CALL   content is: fun [arg0 [arg1 [...]]]
 * COMPOSITE_LIT content is: type [elem0 [elem1 [...]]]
 * FOR    content is: init cond post block
 * IF     content is: init cond then else
 * INDEX  content is: expr [arg0 [arg1 [...]]]
 * RETURN content is: [expr0 [expr1 [...]]]
 * SELECT content is: [clause0 [clause1 [...]]
 * SWITCH content is: init expr [case0 [case1 [...]]
 */
type List struct {
	Atom
	Nodes []Node
}

func (l *List) Len() int {
	return len(l.Nodes)
}

func (l *List) At(i int) Node {
	return l.Nodes[i]
}

func (l *List) End() token.Pos {
	if n := len(l.Nodes); n != 0 {
		return l.Nodes[n-1].End()
	} else {
		return l.Atom.End()
	}
}

func (l *List) String() string {
	if l == nil {
		return "nil"
	}
	var buf strings.Builder
	l.WriteTo(&buf)
	return buf.String()
}

func (l *List) WriteTo(out io.StringWriter) {
	if l == nil {
		out.WriteString("nil")
	} else {
		writeListTo(out, l)
	}
}

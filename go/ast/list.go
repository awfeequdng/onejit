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
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

/**
 * List of nodes. Used for all variable-sized array of nodes,
 * and also for many fixed-size array of nodes, including:
 * BLOCK, CALL, FOR, IF, INDEX, RETURN, SELECT, SWITCH.
 *
 * BLOCK  content is: [stmt0 [stmt1 [...]]
 * CALL   content is: fun [arg0 [arg1 [...]]]
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

func (s *List) Len() int {
	return len(s.Nodes)
}

func (s *List) At(i int) Node {
	return s.Nodes[i]
}

func (s *List) End() token.Pos {
	if n := len(s.Nodes); n != 0 {
		return s.Nodes[n-1].End()
	} else {
		return s.Atom.End()
	}
}

func (s *List) String() string {
	if s == nil {
		return "nil"
	} else {
		return fmt.Sprint(s)
	}
}

func (s *List) Format(out fmt.State, verb rune) {
	if s == nil {
		out.Write(strNil)
	} else {
		formatAsList(out, verb, s)
	}
}

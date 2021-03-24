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

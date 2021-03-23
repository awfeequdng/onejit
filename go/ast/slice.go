/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * slice.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import "github.com/cosmos72/onejit/go/token"

type Slice struct {
	Atom
	Nodes []Node
}

func (s *Slice) Size() int {
	return len(s.Nodes)
}

func (s *Slice) Get(i int) Node {
	return s.Nodes[i]
}

func (s *Slice) End() token.Pos {
	if n := len(s.Nodes); n == 0 {
		return s.Atom.End()
	} else {
		return s.Nodes[n-1].End()
	}
}

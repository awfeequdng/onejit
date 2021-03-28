/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * valuespec.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"fmt"

	"github.com/cosmos72/onejit/go/token"
)

type ValueSpec struct {
	Atom
	Names  *List
	Type   Node
	Values *List
}

func (v *ValueSpec) Len() int {
	return 3
}

func (v *ValueSpec) At(i int) Node {
	return choose3(i, v.Names, v.Type, v.Values)
}

func (v *ValueSpec) End() token.Pos {
	if v.Values != nil {
		return v.Values.End()
	} else if v.Type != nil {
		return v.Type.End()
	} else if v.Names != nil {
		return v.Names.End()
	} else {
		return v.Atom.End()
	}
}

func (v *ValueSpec) String() string {
	if v == nil {
		return "nil"
	} else {
		return fmt.Sprint(v)
	}
}

func (v *ValueSpec) Format(out fmt.State, verb rune) {
	if v == nil {
		out.Write(strNil)
	} else {
		formatAsList(out, verb, v)
	}
}

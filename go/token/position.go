/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * position.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package token

import (
	"github.com/cosmos72/onejit/go/strings"
)

// equivalent to go/token.Pos
type Pos int

// equivalent go/token.NoPos
const NoPos Pos = 0

func (p Pos) IsValid() bool {
	return p != NoPos
}

// equivalent to go/token.Position
type Position struct {
	Filename string
	Offset   int
	Line     int
	Column   int
}

func (pos *Position) IsValid() bool {
	return pos.Line > 0
}

func (pos *Position) String() string {
	var b strings.Builder
	b.Reset()
	b.WriteString(pos.Filename)
	if pos.IsValid() {
		if b.Len() != 0 {
			b.WriteByte(':')
		}
		b.WriteString(tostring(pos.Line))
		if pos.Column != 0 {
			b.WriteByte(':')
			b.WriteString(tostring(pos.Column))
		}
	}
	if b.Len() != 0 {
		return b.String()
	}
	return "-"
}

func tostring(i int) string {
	return strings.IntToString(i)
}

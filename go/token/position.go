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
	"go/token"
)

// wraps go/token.Pos
type Pos token.Pos

// wraps go/token.NoPos
const NoPos = Pos(token.NoPos)

func (p Pos) IsValid() bool {
	return p != NoPos
}

// wraps go/token.Position
type Position token.Position

func (pos *Position) IsValid() bool {
	return pos.Line > 0
}

func (pos *Position) String() string {
	return (*token.Position)(pos).String()
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * node.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import "github.com/cosmos72/onejit/go/token"

type Node interface {
	Op() token.Token
	Size() int      // return number of children
	Get(i int) Node // get i-th child
	Pos() token.Pos
	End() token.Pos
}

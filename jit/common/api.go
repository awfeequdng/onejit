/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2019-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * api.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package common

type Size uintptr // 1, 2, 4, 8 or 16

type Expr interface {
	RegId() RegId
	Kind() Kind
	Const() bool
	Size() Size
	expr() // private marker
}

type noteq [0]func() // not comparable

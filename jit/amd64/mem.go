/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * mem.go
 *
 *  Created on Dec 26, 2020
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	. "github.com/cosmos72/gomacrojit/jit"
)

// convert a general Mem expression to Amd64Mem
// may allocate registers for intermediate expressions
func toAmd64Mem(f *Func, m Mem) Amd64Mem {
	kind := m.Kind()
	switch expr := m.Addr().(type) {
	case *BinaryExpr:
		switch expr.Op() {
		case ADD:
			return addToAmd64Mem(f, expr.X(), expr.Y())
		case SUB:
			return addToAmd64Mem(f, expr.X(), expr.Y())
		}
	}
}

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
 * arm64.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

// convert a generic Expr to a concrete Expr for arm64 architecture.
// may allocate registers for intermediate expressions
func toArm64(expr Expr, toplevel bool, ac *ArchCompiled) Expr {
	Warnf("toArm64(): unimplemented %v", expr)
	return expr
}

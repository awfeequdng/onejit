/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * resolver.go
 *
 *  Created on: Apr 10, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import "github.com/cosmos72/onejit/go/ast"

// resolves symbols i.e. for each identifies finds its declaration
type Resolver struct {
	multiscope
}

// does NOT clear accumulated errors
func (r *Resolver) Init(c *Collector) {
	r.multiscope = c.multiscope
}

func (r *Resolver) Globals(nodes ...ast.Node) {
}

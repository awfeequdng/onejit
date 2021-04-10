/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * error.go
 *
 *  Created on: Apr 10, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

func (c *Checker) ClearWarnings() {
	c.warnings = nil
}

func (c *Checker) ClearErrors() {
	c.errors = nil
}

func (c *Checker) warning(node ast.Node, msg string) {
	c.warnings = append(c.warnings, c.newError(node, msg))
}

func (c *Checker) error(node ast.Node, msg string) {
	c.errors = append(c.errors, c.newError(node, msg))
}

func (c *Checker) newError(node ast.Node, msg string) *scanner.Error {
	err := &scanner.Error{Msg: msg}
	if node != nil {
		err.Pos = c.fileset.Position(node.Pos())
	}
	return err
}

func (c *Checker) errorRedefined(name string, node ast.Node, old *types.Object) {
	pos := c.position(old)
	if pos.IsValid() {
		c.error(node, name+" redeclared in this block\n\tprevious declaration at "+pos.String())
	} else {
		c.error(node, name+" redeclared in this block")
	}
}

func (c *Checker) position(obj *types.Object) (pos token.Position) {
	if d, _ := obj.Value().(*decl); d != nil {
		if node := d.node; node != nil {
			pos = c.fileset.Position(node.Pos())
		}
	}
	return pos
}

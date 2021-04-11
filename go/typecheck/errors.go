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
)

type errors struct {
	fileset *token.FileSet
	warns   []*scanner.Error
	errs    []*scanner.Error
}

func (e *errors) ClearWarnings() {
	e.warns = nil
}

func (e *errors) ClearErrors() {
	e.errs = nil
}

func (e *errors) warning(node ast.Node, msg string) {
	e.warns = append(e.warns, e.newError(node, msg))
}

func (e *errors) error(node ast.Node, msg string) {
	e.errs = append(e.errs, e.newError(node, msg))
}

func (e *errors) newError(node ast.Node, msg string) *scanner.Error {
	err := &scanner.Error{Msg: msg}
	if node != nil {
		err.Pos = e.fileset.Position(node.Pos())
	}
	return err
}

func (e *errors) errorRedefined(name string, node ast.Node, old *Symbol) {
	pos := e.position(old)
	if pos.IsValid() {
		e.error(node, name+" redeclared in this block\n\tprevious declaration at "+pos.String())
	} else {
		e.error(node, name+" redeclared in this block")
	}
}

func (e *errors) position(sym *Symbol) (pos token.Position) {
	if node := sym.node; node != nil {
		pos = e.fileset.Position(node.Pos())
	}
	return pos
}

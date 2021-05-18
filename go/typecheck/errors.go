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
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

type errors struct {
	fileset *token.FileSet
	warns   []*token.Error
	errs    []*token.Error
}

func (e *errors) ClearWarnings() {
	e.warns = nil
}

func (e *errors) ClearErrors() {
	e.errs = nil
}

func (e *errors) Errors() []*token.Error {
	return e.errs
}

func (e *errors) warning(node ast.Node, msg string) {
	e.warns = append(e.warns, e.newError(node, msg))
}

func (e *errors) error(node ast.Node, msg string) {
	e.errs = append(e.errs, e.newError(node, msg))
}

func (e *errors) assign_error(node ast.Node, src *types.Complete, dst *types.Complete) {
	e.error(node, "cannot use type "+src.String()+" as type "+dst.String()+" in assignment")
}

func (e *errors) newError(node ast.Node, msg string) *token.Error {
	err := &token.Error{Msg: msg}
	if node != nil {
		err.Pos = e.fileset.Position(node.Pos())
	}
	return err
}

func (e *errors) errorRedefined(name string, node ast.Node, old *Object) {
	pos := e.position(old)
	if pos.IsValid() {
		e.error(node, name+" redeclared in this block\n\tprevious declaration at "+pos.String())
	} else {
		e.error(node, name+" redeclared in this block")
	}
}

func (e *errors) position(obj *Object) (pos token.Position) {
	if decl := obj.Decl(); decl != nil {
		if node := decl.node; node != nil {
			pos = e.fileset.Position(node.Pos())
		}
	}
	return pos
}

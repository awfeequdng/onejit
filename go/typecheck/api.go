/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * api.go
 *
 *  Created on: Apr 06, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

type TypeMap map[ast.Node]*types.Complete

// typecheck the specified global declarations
func CheckGlobals(fileset *token.FileSet, scope *types.Scope, knownpkgs types.Packages,
	source ...ast.Node) (decls *types.Scope, typemap TypeMap) {

	c := Collector{}
	c.Init(fileset, scope, knownpkgs)
	c.Globals(source...)
	if len(c.errs) != 0 || len(c.warns) != 0 {
		return nil, nil
	}
	r := Resolver{}
	r.Init(&c)
	r.Globals(source...)
	return nil, nil
}

func token2class(tok token.Token) (cls types.Class) {
	switch tok {
	case token.CONST:
		cls = types.ConstObj
	case token.FUNC:
		cls = types.FuncObj
	case token.TYPE:
		cls = types.TypeObj
	case token.VAR:
		cls = types.VarObj
	default:
	}
	return cls
}

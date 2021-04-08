/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * checker.go
 *
 *  Created on: Apr 06, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/types"
)

type (
	objdecl struct {
		cls  types.Class
		typ  ast.Node // can be nil
		init ast.Node // can also be nil, typeAlias or a multi-valued function call.
	}

	objdecls map[string]objdecl
)

func (l objdecl) String() string {
	return l.cls.String()
}

func nodeString(node ast.Node) string {
	if node == nil {
		return "nil"
	}
	return node.String()
}

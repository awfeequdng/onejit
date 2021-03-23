/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * file.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

func (p *Parser) ParseFile() ast.Node {
	var list []ast.Node
	for {
		node := p.Parse()
		if node.Op() == token.EOF {
			break // end of file
		}
		list = append(list, node)
	}
	var pos token.Pos
	if len(list) != 0 {
		pos = list[0].Pos()
	}
	return &ast.Slice{
		Atom: ast.Atom{
			Tok:    token.FILE,
			TokPos: pos,
		},
		Nodes: list,
	}
}

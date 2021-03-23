/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * expr.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

func (p *Parser) parseIdentifierList() ast.Node {
	ret := &ast.Slice{
		Atom: ast.Atom{
			Tok:    token.IDENT_LIST,
			TokPos: p.pos,
		},
	}
	var list []ast.Node
	if p.tok != token.IDENT {
		list = []ast.Node{p.parseBad()}
	} else {
		for {
			list = append(list, p.parseAtom())
			if p.next() != token.COMMA {
				break
			}
			p.next()
		}
	}
	ret.Nodes = list
	return ret
}

func (p *Parser) parseExprList() ast.Node {
	return nil
}

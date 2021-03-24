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

// parse a comma-separated identifier list
func (p *Parser) parseIdentList() *ast.List {
	ret := &ast.List{
		Atom: ast.Atom{
			Tok:    token.IDENTS,
			TokPos: p.pos(),
		},
	}
	var list []ast.Node
	for {
		list = append(list, p.makeIdent())
		if p.tok() != token.IDENT || p.next() != token.COMMA {
			break
		}
		p.next() // skip ','
	}
	ret.Nodes = list
	return ret
}

// parse a (possibly qualified) identifier
func (p *Parser) parseQualifiedIdent() (node ast.Node) {
	node = p.parseIdent()
	if node.Op() != token.IDENT {
		return node
	}
	for {
		if p.tok() != token.PERIOD {
			break
		}
		binary := p.parseBinary()
		if p.tok() != token.IDENT {
			break
		}
		binary.X = node
		binary.Y = p.parseIdent()
		node = binary
	}
	return node
}

// parse an expression
func (p *Parser) parseExpr() ast.Node {
	return nil // TODO
}

// parse a comma-separated expression list
func (p *Parser) parseExprList() *ast.List {
	return nil // TODO
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * parser.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

func (p *Parser) ParseTopLevelDecl() (node ast.Node) {
	switch p.tok() {
	case token.CONST, token.VAR:
		node = p.parseConstOrVarDecl()
	case token.FUNC:
		node = p.parseFuncOrMethodDecl()
	case token.TYPE:
		node = p.parseTypeDecl()
	default:
		node = p.parseBad(errExpectingConstVarFuncOrType)
	}
	return node
}

func (p *Parser) parseConstOrVarDecl() *ast.List {
	list := p.parseList()
	list.Nodes = p.parseValueSpecList()
	return list
}

func (p *Parser) parseImport() ast.Node {
	list := p.parseList()
	list.Nodes = p.parseImportSpecList()
	return list
}

func (p *Parser) parseImportSpecList() (list []ast.Node) {
	return p.parseAnySpecList((*Parser).parseImportSpec)
}

func (p *Parser) parseValueSpecList() (list []ast.Node) {
	return p.parseAnySpecList((*Parser).parseValueSpec)
}

func (p *Parser) parseAnySpecList(parseSpec func(p *Parser) ast.Node) (list []ast.Node) {
	if p.tok() != token.LPAREN {
		return []ast.Node{parseSpec(p)}
	}
	p.next() // skip '('
	for !isLeave(p.tok()) {
		list = append(list, parseSpec(p))

		if p.tok() == token.SEMICOLON {
			p.next() // skip ';'
		}
	}
	return p.leave(list, token.RPAREN)
}

func (p *Parser) parseValueSpec() ast.Node {
	pos := p.pos()
	comment := p.consumeComment()
	identList := p.parseIdentList()
	var typ ast.Node
	var exprList *ast.List
	if !isLeave(p.tok()) {
		if p.tok() != token.ASSIGN {
			typ = p.parseType()
		}
		if p.tok() == token.ASSIGN {
			exprList = p.parseExprList(nil, noEllipsis)
		}
	}
	return &ast.ValueSpec{
		Atom: ast.Atom{
			Tok:     token.VALUE_SPEC,
			TokPos:  pos,
			Comment: comment,
		},
		Names:  identList,
		Type:   typ,
		Values: exprList,
	}
}

func (p *Parser) parseImportSpec() ast.Node {
	pos := p.pos()
	var alias, path ast.Node
	if p.tok() == token.IDENT {
		alias = p.parseAtom(token.IDENT)
	}
	if p.tok() == token.STRING {
		path = p.parseAtom(token.STRING)
	} else {
		path = p.makeBad(errExpectingString)
	}
	return &ast.List{
		Atom: ast.Atom{
			Tok:    token.IMPORT_SPEC,
			TokPos: pos,
		},
		Nodes: []ast.Node{
			alias, path,
		},
	}
}

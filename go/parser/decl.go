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

func isDecl(tok token.Token) bool {
	switch tok {
	case token.CONST, token.FUNC, token.TYPE, token.VAR:
		return true
	default:
		return false
	}
}

func (p *Parser) parseTopLevelDecl() (node ast.Node) {
	switch p.tok {
	case token.CONST, token.VAR:
		node = p.parseConstOrVarDecl(p.tok)
	case token.FUNC:
		node = p.parseFuncOrMethodDecl()
	case token.TYPE:
		node = p.parseTypeDecl()
	default:
		node = p.parseBad()
	}
	return node
}

func (p *Parser) parseConstOrVarDecl(constOrVar token.Token) ast.Node {
	oldpos := p.pos
	tok := p.next()
	ret := &ast.Slice{
		Atom: ast.Atom{
			Tok:    constOrVar,
			TokPos: oldpos,
			TokEnd: p.pos,
		},
	}
	if tok == token.LPAREN {
		ret.Nodes = p.parseSpecList()
	} else {
		ret.Nodes = []ast.Node{p.parseSpec()}
	}
	return ret
}

func (p *Parser) parseSpecList() (list []ast.Node) {
	for {
		if p.next() == token.RPAREN {
			break
		}
		list = append(list, p.parseSpec())

		if p.next() != token.SEMICOLON {
			list = append(list, p.parseBad())
		}
	}
	return list
}

func (p *Parser) parseSpec() ast.Node {
	pos := p.pos
	identifiers := p.parseIdentifierList()
	var typ ast.Node
	var exprlist ast.Node
	if p.tok != token.SEMICOLON {
		if p.tok != token.ASSIGN {
			typ = p.parseType()
		}
		if p.tok == token.ASSIGN {
			exprlist = p.parseExprList()
		}
	}
	return &ast.Slice{
		Atom: ast.Atom{
			Tok:    token.SPEC,
			TokPos: pos,
		},
		Nodes: []ast.Node{
			identifiers, typ, exprlist,
		},
	}
}

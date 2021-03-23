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

func isLeave(tok token.Token) bool {
	switch tok {
	case token.ILLEGAL, token.EOF, token.SEMICOLON,
		token.RPAREN, token.RBRACK, token.RBRACE:
		return true
	default:
		return false
	}
}

func (p *Parser) parseTopLevelDecl() (node ast.Node) {
	switch p.tok {
	case token.CONST, token.VAR:
		node = p.parseGenDecl()
	case token.FUNC:
		node = p.parseFuncOrMethodDecl()
	case token.TYPE:
		node = p.parseTypeDecl()
	default:
		node = p.makeBad()
	}
	return node
}

func (p *Parser) parseGenDecl() ast.Node {
	ret := p.makeSlice()
	if p.next() == token.LPAREN {
		ret.Nodes = p.parseGenSpecList()
	} else {
		ret.Nodes = []ast.Node{p.parseGenSpec()}
	}
	return ret
}

func (p *Parser) parseImport() ast.Node {
	ret := p.makeSlice()
	if p.next() == token.LPAREN {
		ret.Nodes = p.parseImportSpecList()
	} else {
		ret.Nodes = []ast.Node{p.parseImportSpec()}
	}
	return ret
}

func (p *Parser) parseGenSpecList() (list []ast.Node) {
	p.next()
	for {
		if tok := p.tok; isLeave(tok) {
			if tok != token.RPAREN {
				list = append(list, p.makeBad())
			}
			break
		}
		list = append(list, p.parseGenSpec())

		if p.tok == token.SEMICOLON {
			p.next()
		}
	}
	return list
}

func (p *Parser) parseImportSpecList() (list []ast.Node) {
	p.next()
	for {
		if tok := p.tok; isLeave(tok) {
			if tok != token.RPAREN {
				list = append(list, p.makeBad())
			}
			break
		}
		list = append(list, p.parseImportSpec())

		if p.tok == token.SEMICOLON {
			p.next()
		}
	}
	return list
}

func (p *Parser) parseGenSpec() ast.Node {
	pos := p.pos
	identList := p.parseIdentList()
	var typ ast.Node
	var exprList ast.Node
	if !isLeave(p.tok) {
		if p.tok != token.ASSIGN {
			typ = p.parseType()
		}
		if p.tok == token.ASSIGN {
			exprList = p.parseExprList()
		}
	}
	if false {
		typ = &ast.Slice{
			Atom:  ast.Atom{Tok: token.TYPE},
			Nodes: []ast.Node{typ},
		}
	}
	return &ast.Slice{
		Atom: ast.Atom{
			Tok:    token.SPEC,
			TokPos: pos,
		},
		Nodes: []ast.Node{
			identList, typ, exprList,
		},
	}
}

func (p *Parser) parseImportSpec() ast.Node {
	pos := p.pos
	var alias, path ast.Node
	if p.tok == token.IDENT {
		alias = p.makeAtom()
		p.next()
	}
	if p.tok == token.STRING {
		path = p.makeAtom()
	} else {
		path = p.makeBad()
	}
	return &ast.Slice{
		Atom: ast.Atom{
			Tok:    token.IMPORT_SPEC,
			TokPos: pos,
		},
		Nodes: []ast.Node{
			alias, path,
		},
	}
}

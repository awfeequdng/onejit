/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * stmt.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

func (p *Parser) parseStmtList() *ast.List {
	list := p.makeList()
	list.Tok = token.BLOCK

	var nodes []ast.Node
	for {
		tok := p.tok()
		if tok == token.CASE || tok == token.DEFAULT || isLeave(tok) {
			break
		}
		nodes = append(nodes, p.ParseStmt())
		if p.tok() == token.SEMICOLON {
			p.next() // skip ';'
		} else {
			break
		}
	}
	list.Nodes = nodes
	return list
}

func (p *Parser) ParseStmt() (node ast.Node) {
	switch p.tok() {
	case token.SEMICOLON:
		// node = nil
	case token.LBRACE:
		node = p.parseBlock()
	case token.CONST, token.VAR:
		node = p.parseValueDecl()
	case token.TYPE:
		node = p.parseTypeDecl()
	case token.BREAK:
		node = p.parseBreak()
	case token.CONTINUE:
		node = p.parseContinue()
	case token.DEFER:
		node = p.parseDefer()
	case token.FALLTHROUGH:
		node = p.parseFallthrough()
	case token.FOR:
		node = p.parseFor()
	case token.GO:
		node = p.parseGo()
	case token.GOTO:
		node = p.parseGoto()
	case token.IF:
		node = p.parseIf()
	case token.RETURN:
		node = p.parseReturn()
	case token.SELECT:
		node = p.parseSelect()
	case token.SWITCH:
		node = p.parseSwitch()
	default:
		// TODO:
		// parse SendStmt i.e. expr '<-' expr
		// parse IncDecStmt, Assignment, ShortVarDecl
		// parse LabeledStmt i.e. ident ':' statement
		node = p.ParseExpr()
	}
	return node
}

func (p *Parser) parseBlock() *ast.List {
	if p.tok() != token.LBRACE {
		list := p.makeList()
		list.Tok = token.BLOCK
		list.Nodes = []ast.Node{p.makeBad(token.LBRACE)}
		return list
	}
	p.next() // skip '{'
	list := p.parseStmtList()

	if p.tok() == token.RBRACE {
		p.next() // skip '}'
	} else {
		list.Nodes = append(list.Nodes, p.makeBad(token.RBRACE))
	}
	return list
}

func (p *Parser) parseBreak() ast.Node {
	return nil // TODO
}

func (p *Parser) parseContinue() ast.Node {
	return nil // TODO
}

func (p *Parser) parseDefer() ast.Node {
	return nil // TODO
}

func (p *Parser) parseFallthrough() ast.Node {
	return nil // TODO
}

func (p *Parser) parseFor() ast.Node {
	return nil // TODO
}

func (p *Parser) parseGo() ast.Node {
	return nil // TODO
}

func (p *Parser) parseGoto() ast.Node {
	return nil // TODO
}

func (p *Parser) parseIf() ast.Node {
	return nil // TODO
}

func (p *Parser) parseReturn() ast.Node {
	return nil // TODO
}

func (p *Parser) parseSelect() ast.Node {
	return nil // TODO
}

func (p *Parser) parseSwitch() ast.Node {
	return nil // TODO
}

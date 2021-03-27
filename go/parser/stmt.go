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

func (p *Parser) parseStmtList() []ast.Node {
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
	return nodes
}

// parse a single statement. does NOT consume the following ';' if present
func (p *Parser) ParseStmt() (node ast.Node) {
	switch p.tok() {
	case token.LBRACE:
		node = p.parseBlock()
	case token.CONST, token.VAR:
		node = p.parseConstOrVarDecl()
	case token.TYPE:
		node = p.parseTypeDecl()
	case token.BREAK, token.CONTINUE:
		node = p.parseBreakOrContinue()
	case token.DEFER, token.GO:
		node = p.parseDeferOrGo()
	case token.FALLTHROUGH:
		node = p.parseFallthrough()
	case token.FOR:
		node = p.parseFor()
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
		node = p.parseSimpleStmt(noRange)
	}
	return node
}

func (p *Parser) parseBlock() *ast.List {
	list := p.makeList()
	list.Tok = token.BLOCK
	list.Nodes = p.enter(nil, token.LBRACE)
	if list.Nodes == nil {
		list.Nodes = p.parseStmtList()
		list.Nodes = p.leave(list.Nodes, token.RBRACE)
	}
	return list
}

type simpleStmtFlag int

const (
	noRange simpleStmtFlag = iota
	allowRange
)

func (p *Parser) parseSimpleStmt(flag simpleStmtFlag) (node ast.Node) {
	if p.tok() == token.SEMICOLON {
		// node = nil
	} else {
		// TODO:
		// parse SendStmt i.e. expr '<-' expr
		// parse IncDecStmt, Assignment, ShortVarDecl
		// parse LabeledStmt i.e. ident ':' statement
		// parse Assignment i.e. ident, ... '=' expr, ...
		// parse ShortVarDecl i.e. ident, ... ':=' expr, ...
		// parse RangeStmt i.e. ident, ... ':=' 'range' expr
		node = p.ParseExpr()
	}
	return node
}

func (p *Parser) parseBreakOrContinue() *ast.Unary {
	unary := p.parseUnary() // also skips 'break' or 'continue'
	if p.tok() == token.IDENT {
		unary.X = p.parseIdent() // label
	}
	return unary
}

func (p *Parser) parseDeferOrGo() *ast.Unary {
	unary := p.parseUnary() // also skips 'defer'
	unary.X = p.ParseExpr()
	return unary
}

func (p *Parser) parseFallthrough() ast.Node {
	return p.parseAtom(p.tok())
}

func (p *Parser) parseFor() *ast.List {
	list := p.parseList() // also skips 'for'
	var init ast.Node
	if p.tok() != token.LBRACE {
		init = p.parseSimpleStmt(allowRange)
		if init.Op() == token.RANGE {
			list.Tok = token.RANGE
			list.Nodes = []ast.Node{
				init.At(0), init.At(1), p.parseBlock(),
			}
			return list
		}
	}
	nodes := make([]ast.Node, 4)
	if p.tok() == token.SEMICOLON {
		p.next()
		nodes[0] = init
		nodes[1] = p.ParseExpr() // condition
		if p.tok() == token.SEMICOLON {
			p.next()
			nodes[2] = p.parseSimpleStmt(noRange) // post
		} else {
			nodes[2] = p.parseBad(token.SEMICOLON)
		}
	} else {
		nodes[1] = init // TODO check that it's nil or an expression
	}
	nodes[3] = p.parseBlock()
	list.Nodes = nodes
	return list
}

func (p *Parser) parseGoto() *ast.Unary {
	unary := p.parseUnary()  // also skips 'goto'
	unary.X = p.parseIdent() // label
	return unary
}

func (p *Parser) parseIf() *ast.List {
	list := p.parseList() // also skips 'if'
	nodes := make([]ast.Node, 4)
	init := p.parseSimpleStmt(noRange)
	if p.tok() == token.SEMICOLON {
		p.next()
		nodes[0] = init
		nodes[1] = p.ParseExpr()
	} else {
		nodes[1] = init // TODO check that it's an expression
	}
	nodes[2] = p.parseBlock()
	if p.tok() == token.ELSE {
		p.next()
		if p.tok() == token.IF {
			nodes[3] = p.parseIf()
		} else {
			nodes[3] = p.parseBlock()
		}
	}
	list.Nodes = nodes
	return list
}

func (p *Parser) parseReturn() *ast.List {
	pos := p.pos()
	p.next() // skip 'return'
	list := p.parseExprList(nil, noEllipsis)
	list.Tok = token.RETURN
	list.TokPos = pos
	return list
}

func (p *Parser) parseSelect() *ast.List {
	return nil // TODO
}

func (p *Parser) parseSwitch() *ast.List {
	return nil // TODO
}

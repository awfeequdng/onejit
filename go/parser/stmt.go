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
		nodes = append(nodes, p.parseStmt(allowCompositeLit))
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
	return p.parseStmt(allowCompositeLit)
}

// parse a single statement. does NOT consume the following ';' if present
func (p *Parser) parseStmt(flag exprFlag) (node ast.Node) {
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
		node = p.parseSimpleStmt(flag)
	}
	if p.tok() == token.COLON && node != nil && node.Op() == token.IDENT {
		binary := p.parseBinary()
		binary.Tok = token.LABEL
		binary.X = node
		binary.Y = p.parseStmt(flag)
		node = binary
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

func (p *Parser) parseSimpleStmt(flag exprFlag) ast.Node {
	if p.tok() == token.SEMICOLON {
		return nil
	}
	node := p.parseExprOrType(token.LowestPrec, flag)
	switch p.tok() {
	case token.ARROW: // SendStmt
		binary := p.parseBinary()
		binary.X = node
		binary.Y = p.ParseExpr()
		node = binary
	case token.INC, token.DEC: // IncDecStmt
		unary := p.parseUnary()
		unary.X = node
		node = unary
	case token.ASSIGN, token.DEFINE, token.COMMA:
		node = p.parseAssign(node, flag)
	}
	return node
}

// parse Assignment i.e. expr, ... '=' expr, ...
// parse ShortVarDecl i.e. ident, ... ':=' expr, ...
// parse RangeStmt i.e. ident, ... ':=' 'range' expr
func (p *Parser) parseAssign(first ast.Node, flag exprFlag) ast.Node {
	if p.tok() == token.COMMA {
		p.next()
	}
	left := p.parseExprList(first, noEllipsis)

	if tok := p.tok(); tok != token.ASSIGN && tok != token.DEFINE {
		left.Nodes = append(left.Nodes, p.parseBad(errExpectingAssignDefineOrComma))
		return left
	}
	assign := p.parseBinary() // also skips '=' or ':='
	var stmt *ast.Unary
	if flag&allowRange != 0 && p.tok() == token.RANGE {
		stmt = p.parseUnary() // also skips 'range'
		stmt.X = assign
	}
	assign.X = left
	assign.Y = p.parseExprList(nil, noEllipsis)
	if stmt == nil {
		return assign
	}
	return stmt
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
				init.At(0), p.parseBlock(),
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

func (p *Parser) parseSendStmt(left ast.Node) *ast.Binary {
	return nil // TODO
}

func (p *Parser) parseSelect() *ast.List {
	return nil // TODO
}

func (p *Parser) parseSwitch() *ast.List {
	return nil // TODO
}

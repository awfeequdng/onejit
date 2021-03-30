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

// parse a list of statements, including the terminating ';'
func (p *Parser) parseStmtList(prefix []ast.Node) []ast.Node {
	nodes := prefix
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
		list.Nodes = p.parseStmtList(nil)
		list.Nodes = p.leave(list.Nodes, token.RBRACE)
	}
	return list
}

func (p *Parser) parseSimpleStmt(flag exprFlag) ast.Node {
	if p.tok() == token.SEMICOLON || p.tok() == token.LBRACE || isLeave(p.tok()) {
		return nil
	}
	list := p.parseExprList(nil, flag&(allowCompositeLit|allowTypeSwitch))
	var node ast.Node = list
	n := list.Len()
	if n == 1 {
		node = list.At(0)
	}
	switch tok := p.tok(); tok {
	case token.ARROW: // SendStmt
		if n != 1 {
			node = p.makeBadNode(node, errExpectedOneExpr)
		}
		binary := p.parseBinary()
		binary.X = node
		binary.Y = p.ParseExpr()
		node = binary
	case token.INC, token.DEC: // IncDecStmt
		if n != 1 {
			node = p.makeBadNode(node, errExpectedOneExpr)
		}
		unary := p.parseUnary()
		unary.X = node
		node = unary
	case token.RANGE:
		if flag&allowRange != 0 {
			node = p.parseRangeStmt()
		}
	default:
		if isAssign(tok) {
			node = p.parseAssign(list, flag)
		}
	}
	return node
}

func (p *Parser) parseRangeStmt() *ast.Unary {
	unary := p.parseUnary() // also skips 'range'
	unary.X = p.parseExprOrType(token.LowestPrec, noCompositeLit)
	return unary
}

// parse Assignment i.e. expr, ... 'op=' expr, ...
// parse ShortVarDecl i.e. ident, ... ':=' expr, ...
// parse RangeStmt i.e. ident, ... ':=' 'range' expr
// left contains everything before 'op=' or ':='
func (p *Parser) parseAssign(left *ast.List, flag exprFlag) ast.Node {
	assign := p.parseBinary() // also skips '=' or ':='
	var stmt *ast.Unary
	if flag&allowRange != 0 && p.tok() == token.RANGE {
		stmt = p.parseUnary() // also skips 'range'
		stmt.X = assign
	}
	assign.X = left
	assign.Y = p.parseExprList(nil, flag&(allowCompositeLit|allowTypeSwitch))
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
	switch p.tok() {
	case token.LBRACE:
		// nothing to do
	case token.RANGE:
		init = p.parseRangeStmt()
		list.Tok = token.RANGE
		list.Nodes = []ast.Node{
			init, p.parseBlock(),
		}
		return list
	default:
		init = p.parseSimpleStmt(allowRange)
		if init != nil && init.Op() == token.RANGE {
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
		if p.tok() != token.SEMICOLON && p.tok() != token.LBRACE && !isLeave(p.tok()) {
			nodes[1] = p.ParseExpr() // condition
		}
		if p.tok() == token.SEMICOLON {
			p.next()
			nodes[2] = p.parseSimpleStmt(noRange) // post, may be nil
		} else {
			nodes[2] = p.parseBad(token.SEMICOLON)
		}
	} else if init != nil {
		if isSimpleStmt(init) {
			init = p.makeBadNode(init, errExpectingExpr)
		}
		nodes[1] = init
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
		nodes[1] = p.parseExprOrType(token.LowestPrec, noCompositeLit)
	} else {
		if isSimpleStmt(init) {
			init = p.makeBadNode(init, errExpectingExpr)
		}
		nodes[1] = init
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
	list := p.parseExprList(nil, allowCompositeLit)
	list.Tok = token.RETURN
	list.TokPos = pos
	return list
}

func (p *Parser) parseSelect() *ast.List {
	list := p.parseList() // also skips 'select'
	var nodes []ast.Node
	nodes = p.enter(nodes, token.LBRACE)
	for p.tok() == token.CASE || p.tok() == token.DEFAULT {
		nodes = append(nodes, p.parseSelectCase())
	}
	nodes = p.leave(nodes, token.RBRACE)
	list.Nodes = nodes
	return list
}

func (p *Parser) parseSelectCase() ast.Node {
	tok := p.tok()
	list := p.parseList() // also skips 'case' or 'default'
	var nodes []ast.Node
	if tok == token.CASE {
		node := p.parseSimpleStmt(noRange)
		if !isSendOrRecvStmt(node) {
			node = p.makeBadNode(node, errSelectCaseNotSendOrRecv)
		}
		nodes = append(nodes, node)
	}
	if p.tok() == token.COLON {
		p.next() // skip ':'
		list.Nodes = p.parseStmtList(nodes)
	} else {
		list.Nodes = append(nodes, p.parseBad(token.COLON))
	}
	return list
}

func isSendOrRecvStmt(node ast.Node) bool {
	switch node.Op() {
	case token.ARROW:
		return true
	case token.ASSIGN, token.DEFINE:
		return node.Len() == 2 && node.At(1).Op() == token.EXPRS &&
			node.At(1).Len() == 1 && node.At(1).At(0).Op() == token.ARROW
	default:
		return false
	}
}

func (p *Parser) parseSwitch() *ast.List {
	list := p.parseList() // also skips 'switch'
	var init, expr ast.Node
	if p.tok() != token.SEMICOLON && p.tok() != token.LBRACE {
		init = p.parseSimpleStmt(allowTypeSwitch)
		if p.tok() == token.SEMICOLON {
			p.next() // skip ';'
			if p.tok() != token.LBRACE {
				expr = p.parseSimpleStmt(allowTypeSwitch)
				if isTypeSwitchStmt(expr) {
					list.Tok = token.TYPESWITCH
				}
			}
		} else {
			// only one statement before '{'
			if isSimpleStmt(init) {
				if isTypeSwitchStmt(init) {
					list.Tok = token.TYPESWITCH
				} else {
					init = p.makeBadNode(init, errExpectingExpr)
				}
			}
			expr = init
			init = nil
		}
	}
	if init != nil && isTypeSwitchStmt(init) {
		list.Tok = token.TYPESWITCH
		if expr != nil {
			expr = p.makeBadNode(init, token.LBRACE)
		}
	}
	nodes := []ast.Node{init, expr}

	nodes = p.enter(nodes, token.LBRACE)
	for p.tok() == token.CASE || p.tok() == token.DEFAULT {
		nodes = append(nodes, p.parseSwitchCase())
	}
	nodes = p.leave(nodes, token.RBRACE)
	list.Nodes = nodes
	return list
}

func isTypeSwitchStmt(node ast.Node) bool {
	switch node.Op() {
	case token.ASSIGN, token.DEFINE:
		if node.Len() == 2 && node.At(1).Op() == token.EXPRS && node.At(1).Len() == 1 {
			node = node.At(1).At(0)
			return node.Op() == token.TYPE_ASSERT && node.Len() == 2 && node.At(1).Op() == token.TYPE
		}
	}
	return false
}

func (p *Parser) parseSwitchCase() ast.Node {
	tok := p.tok()
	list := p.parseList() // also skips 'case' or 'default'
	var nodes []ast.Node
	if tok == token.CASE {
		nodes = append(nodes, p.parseExprList(nil, noEllipsis))
	}
	if p.tok() == token.COLON {
		p.next() // skip ':'
		list.Nodes = p.parseStmtList(nodes)
	} else {
		list.Nodes = append(nodes, p.parseBad(token.COLON))
	}
	return list
}

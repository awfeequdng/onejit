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
			Tok:    token.NAMES,
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

// parse a comma-separated expression list
func (p *Parser) parseExprList(allowEllipsis bool) *ast.List {
	pos := p.pos()
	var list []ast.Node
	for {
		list = append(list, p.ParseExpr())
		if p.tok() != token.COMMA {
			break
		}
		p.next() // skip ','
	}
	if n := len(list); n != 0 && allowEllipsis && p.tok() == token.ELLIPSIS {
		unary := p.parseUnary() // skips '...'
		unary.X = list[n-1]
		list[n-1] = unary
	}
	return &ast.List{
		Atom:  ast.Atom{Tok: token.EXPRS, TokPos: pos},
		Nodes: list,
	}
}

func (p *Parser) ParseExpr() ast.Node {
	return p.parseExprOrType(token.LowestPrec)
}

func (p *Parser) parseExprOrType(prec int) ast.Node {
	node := p.parseUnaryExpr()
	for {
		tokPrec := p.tok().Precedence()
		if tokPrec <= prec {
			break
		}
		// found binary operator with higher precedence
		binary := p.parseBinary()
		binary.X = node
		binary.Y = p.parseExprOrType(tokPrec)
		node = binary
	}
	return node
}

func (p *Parser) parseUnaryExpr() ast.Node {
	if !isUnary(p.tok()) {
		return p.parsePrimaryExpr()
	}
	unary := p.parseUnary()
	unary.X = p.parseUnaryExpr()
	return unary
}

func (p *Parser) parsePrimaryExpr() ast.Node {
	node := p.parseOperandExpr()
	for {
		switch p.tok() {
		case token.LPAREN:
			node = p.parseCallExpr(node)
		case token.LBRACK:
			node = p.parseIndexOrSlice(node)
		case token.LBRACE:
			node = p.parseCompositeLit(node)
		case token.PERIOD:
			// either node . identifier
			// or node . ( type )
			pos := p.pos()
			switch p.next() {
			case token.IDENT:
				node = p.parseSelector(node, pos)
				continue // loop
			case token.LPAREN:
				node = p.parseTypeAssert(node)
			default:
				node = p.makeBadNode(node, errExpectingIdentOrLparen)
			}
		default:
		}
		break
	}
	return node
}

func (p *Parser) parseOperandExpr() (node ast.Node) {
	switch p.tok() {
	case token.INT, token.FLOAT, token.IMAG, token.CHAR, token.STRING:
		node = p.parseAtom(p.tok())
	case token.LPAREN:
		p.next() // skip '('
		node = p.ParseExpr()
		node = p.leaveNode(node, token.RPAREN)
	case token.IDENT:
		node = p.parseIdent()
	case token.FUNC:
		node = p.parseFunctionLit()
	case token.CHAN:
		node = p.parseBad(errExpectingExprOrType)
	default:
		node = p.parseType()
	}
	return node
}

// last argument may be followed by '...'
// if no '...' and arguments len = 1, may also be a type conversion
func (p *Parser) parseCallExpr(fun ast.Node) *ast.Binary {
	call := p.parseBinary() // skips '('
	call.Tok = token.CALL
	call.X = fun
	args := p.parseExprList(true)
	args.Nodes = p.leave(args.Nodes, token.RPAREN)
	call.Y = args
	return call
}

func (p *Parser) parseCompositeLit(typ ast.Node) ast.Node {
	return typ // TODO
}

func (p *Parser) parseIndexOrSlice(left ast.Node) *ast.Binary {
	binary := p.parseBinary() // also skips '['
	binary.X = left
	list := p.makeList()
	list.Tok = token.EXPRS
	binary.Y = list

	var nodes []ast.Node
	var sep token.Token
	for !isLeave(p.tok()) {
		if p.tok() == token.COLON {
			// no expression
			if sep == token.COMMA {
				break
			}
			nodes = append(nodes, nil)
			sep = token.COLON
			p.next() // skip ':'
			continue
		}
		nodes = append(nodes, p.parseExprOrType(token.LowestPrec))
		var want token.Token
		if tok := p.tok(); tok == token.COMMA || tok == token.COLON {
			want = tok
			p.next() // skip ',' or ':'
		} else {
			break
		}
		if sep == 0 {
			sep = want
		} else if sep != want {
			break
		}
	}
	if nodes == nil {
		nodes = []ast.Node{p.makeBad(errExpectingExpr)}
	}
	nodes = p.leave(nodes, token.RBRACK)
	if sep == token.COMMA {
		binary.Tok = token.INDEX
	} else {
		binary.Tok = token.SLICE_EXPR
	}
	list.Nodes = nodes
	return binary
}

func (p *Parser) parseSelector(left ast.Node, pos token.Pos) *ast.Binary {
	return &ast.Binary{
		Atom: ast.Atom{Tok: token.PERIOD, TokPos: pos},
		X:    left,
		Y:    p.parseIdent(),
	}
}

func (p *Parser) parseTypeAssert(left ast.Node) *ast.Binary {
	binary := p.parseBinary() // also skips '('
	binary.Tok = token.TYPE_ASSERT
	binary.X = left
	typ := p.parseType()
	binary.Y = p.leaveNode(typ, token.RPAREN)
	return binary
}

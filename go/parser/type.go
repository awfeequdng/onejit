/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * type.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

func (p *Parser) parseTypeDecl() *ast.List {
	list := p.parseList()
	list.Nodes = p.parseTypeSpecList()
	return list
}

func (p *Parser) parseTypeSpecList() (list []ast.Node) {
	return p.parseAnySpecList((*Parser).parseTypeSpec)
}

func (p *Parser) parseTypeSpec() ast.Node {
	tok, pos := token.DEFINE, p.pos()
	name := p.parseIdent()
	if p.tok() == token.ASSIGN {
		// type alias
		tok, pos = token.ASSIGN, p.pos()
		p.next() // skip '='
	}
	typ := p.parseType()
	binary := &ast.Binary{
		Atom: ast.Atom{Tok: tok, TokPos: pos},
		X:    name,
		Y:    typ,
	}
	if tok == token.ASSIGN && p.mode&TypeAlias == 0 {
		binary = p.makeBinaryBad(binary, errTypeAlias)
	}
	return binary
}

// parse a type
func (p *Parser) parseType() (node ast.Node) {
	switch p.tok() {
	case token.LBRACK:
		node = p.parseArrayType()
	case token.ARROW, token.CHAN:
		node = p.parseChannelType()
	case token.FUNC:
		node = p.parseFunctionType()
	case token.INTERFACE:
		node = p.parseInterfaceType()
	case token.MAP:
		node = p.parseMapType()
	case token.IDENT:
		node = p.parseQualifiedIdent()
	case token.STRUCT:
		node = p.parseStructType()
	case token.MUL:
		node = p.parsePointerType()
	case token.LPAREN:
		p.next() // skip '('
		node = p.parseType()
		if p.tok() == token.RPAREN {
			p.next()
		} else {
			node = p.makeBinaryBad(node, token.RPAREN)
		}
	default:
		node = p.parseBad(errExpectingType)
	}
	return node
}

// parse a type, possibly with prefix "..."
func (p *Parser) parseTypeMaybeEllipsis() (node ast.Node) {
	if p.tok() != token.ELLIPSIS {
		return p.parseType()
	}
	ellipsis := p.parseUnary()
	ellipsis.X = p.parseType()
	return ellipsis
}

// parse array or slice type
func (p *Parser) parseArrayType() ast.Node {
	pos := p.pos()
	var length ast.Node
	switch p.next() { // skip '[
	case token.RBRACK:
		// slice. it does not declare its length
	case token.ELLIPSIS:
		length = p.parseAtom(token.ELLIPSIS)
	default:
		length = p.ParseExpr()
	}
	var elem ast.Node
	if p.tok() != token.RBRACK {
		elem = p.makeBad(token.RBRACK.String())
	} else {
		p.next() // skip ']'
		elem = p.parseType()
	}
	return &ast.Binary{
		Atom: ast.Atom{Tok: token.ARRAY, TokPos: pos},
		X:    length,
		Y:    elem,
	}
}

func (p *Parser) parseChannelType() ast.Node {
	var dir *ast.Atom
	if p.tok() == token.ARROW {
		dir = p.parseAtom(token.RECV_DIR)
	}
	if p.tok() != token.CHAN {
		return p.parseBad(errExpectingChan)
	}
	binary := p.parseBinary() // skips 'chan'
	if dir == nil && p.tok() == token.ARROW {
		dir = p.parseAtom(token.SEND_DIR)
	}
	binary.X = dir
	binary.Y = p.parseType() // element type
	return binary
}

func (p *Parser) parseMapType() ast.Node {
	binary := p.parseBinary()
	if p.tok() != token.LBRACK {
		binary.X = p.makeBad(token.LBRACK.String())
		return binary
	}
	p.next()                 // skip '['
	binary.X = p.parseType() // key type
	if p.tok() != token.RBRACK {
		binary.Y = p.makeBad(token.RBRACK.String())
		return binary
	}
	p.next()                 // skip ']'
	binary.Y = p.parseType() // element type
	return binary
}

func (p *Parser) parsePointerType() ast.Node {
	unary := p.parseUnary()
	unary.X = p.parseType()
	return unary
}

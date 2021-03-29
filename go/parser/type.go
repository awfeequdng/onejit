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

// parse a 'type ( name1 spec1; name2 spec2 ...)' declaration
func (p *Parser) parseTypeDecl() *ast.List {
	list := p.parseList()
	list.Nodes = p.parseTypeSpecList()
	return list
}

func (p *Parser) parseTypeSpecList() (list []ast.Node) {
	return p.parseAnySpecList((*Parser).parseTypeSpec)
}

func (p *Parser) parseTypeSpec() (node ast.Node) {
	tok, pos := token.DEFINE, p.pos()
	name := p.parseIdent()
	if p.tok() == token.ASSIGN {
		// type alias
		tok, pos = token.ASSIGN, p.pos()
		p.next() // skip '='
	}
	typ := p.ParseType(true)
	node = &ast.Binary{
		Atom: ast.Atom{Tok: tok, TokPos: pos},
		X:    name,
		Y:    typ,
	}
	if tok == token.ASSIGN && p.mode&TypeAlias == 0 {
		node = p.makeBadNode(node, errTypeAlias)
	}
	return node
}

// parse a comma-separated list of types
func (p *Parser) parseTypeList(prefix []ast.Node) []ast.Node {
	nodes := prefix
	for !isLeave(p.tok()) {
		nodes = append(nodes, p.parseType())
		if p.tok() == token.COMMA {
			p.next() // skip ','
		} else {
			break
		}
	}
	return nodes
}

// parse a type
func (p *Parser) parseType() ast.Node {
	return p.ParseType(false)
}

// parse a type.
// if isTypeDecl is true and p.mode & Generics != 0,
// then parse also any generic type declaration prefix '[T1 constraint1, T2 constraint2 ...]'
func (p *Parser) ParseType(isTypeDecl bool) (node ast.Node) {
	switch p.tok() {
	case token.IDENT:
		node = p.parseQualifiedIdent()
		if p.tok() == token.LBRACK {
			node = p.parseMaybeGenericInstantiation(node)
		}
	case token.LPAREN:
		p.next() // skip '('
		node = p.parseType()
		node = p.leaveNode(node, token.RPAREN)
	case token.LBRACK:
		node = p.parseArrayOrGenericType(isTypeDecl)
	case token.ARROW, token.CHAN:
		node = p.parseChannelType()
	case token.FUNC:
		node = p.parseFunctionType()
	case token.INTERFACE:
		node = p.parseInterfaceType()
	case token.MAP:
		node = p.parseMapType()
	case token.STRUCT:
		node = p.parseStructType()
	case token.MUL:
		node = p.parsePointerType()
	default:
		return p.parseBad(errExpectingType)
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

// parse array type, slice type or generic type declaration
func (p *Parser) parseArrayOrGenericType(isDeclaration bool) ast.Node {
	pos := p.pos()
	var arg ast.Node
	switch p.next() { // skip '[
	case token.RBRACK:
		// slice. it does not declare its length
	case token.ELLIPSIS:
		arg = p.parseAtom(token.ELLIPSIS)
	default:
		arg = p.ParseExpr()
	}
	if p.tok() == token.RBRACK {
		p.next() // skip ']'
	} else if !isDeclaration || arg == nil || arg.Op() != token.IDENT {
		arg = p.leaveNode(arg, token.RBRACK)
	} else {
		// '[T1 constraint1, T2 constraint2 ...]' generic type declaration
		// and we already parsed [T1
		gen := p.parseGenericParams(arg)
		gen.Type = p.parseType()
		return gen
	}
	return &ast.Binary{
		Atom: ast.Atom{Tok: token.ARRAY, TokPos: pos},
		X:    arg,
		Y:    p.parseType(),
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
		binary.X = p.makeBad(token.LBRACK)
		return binary
	}
	p.next()                 // skip '['
	binary.X = p.parseType() // key type
	if p.tok() != token.RBRACK {
		binary.Y = p.makeBad(token.RBRACK)
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

// parse the fragment '[T1 constraint1, T2 constraint2 ...]' in a generic type declaration
// if t1 != nil then it means we already parsed '[T1'
func (p *Parser) parseGenericParams(t1 ast.Node) *ast.GenericType {
	gen := &ast.GenericType{
		Atom: ast.Atom{Tok: token.GENERIC, TokPos: p.pos()},
		Params: &ast.List{
			Atom: ast.Atom{Tok: token.PARAMS, TokPos: p.pos()},
		},
	}
	var nodes []ast.Node
	if t1 == nil {
		nodes = p.enter(nil, token.LBRACK)
		if nodes != nil {
			gen.Params.Nodes = nodes
			return gen
		}
	}
	if p.mode&Generics == 0 {
		nodes = append(nodes, p.makeBad(errGenerics))
	}
	for !isLeave(p.tok()) {
		nodes = append(nodes, p.parseGenericParamDecl(t1))
		t1 = nil
		if p.tok() == token.COMMA {
			p.next() // skip ','
		} else {
			break
		}
	}
	if nodes == nil {
		nodes = append(nodes, p.makeBad(errEmptyTypeParams))
	}
	nodes = p.leave(nodes, token.RBRACK)
	gen.Params.Nodes = nodes
	return gen
}

// parse the fragment 'T1, T2, T3 constraint' in a generic type declaration
func (p *Parser) parseGenericParamDecl(t1 ast.Node) *ast.Field {
	field := &ast.Field{
		Atom: ast.Atom{Tok: token.FIELD, TokPos: p.pos()},
		Names: &ast.List{
			Atom: ast.Atom{Tok: token.NAMES, TokPos: p.pos()},
		},
	}
	var list []ast.Node
	for {
		if t1 != nil {
			// we already parsed T1
			list = append(list, t1)
			t1 = nil
		} else if p.tok() == token.IDENT {
			list = append(list, p.parseIdent())
		} else {
			break
		}
		if p.tok() == token.COMMA {
			p.next() // skip ','
		} else {
			break
		}
	}
	field.Names.Nodes = list
	field.Type = p.parseType() // constraint
	return field
}

// parse the suffix '[T1, T2...]' in a generic type instantiation
// tricky case: the suffixes '[ ]' '[ INT' must NOT be parsed, they belong to the next node
func (p *Parser) parseMaybeGenericInstantiation(typ ast.Node) ast.Node {
	atom := p.curr
	p.consumeComment()
	p.next() // skip '['
	switch p.tok() {
	case token.RBRACK, token.INT:
		// put back '[' and current token into the token stream
		p.unread(&atom)
		return typ
	}
	list := &ast.List{Atom: atom}
	list.Tok = token.INDEX
	nodes := []ast.Node{typ}
	if p.mode&Generics == 0 {
		nodes = append(nodes, p.makeBad(errGenerics))
	}
	nodes = p.parseTypeList(nodes)
	nodes = p.leave(nodes, token.RBRACK)
	list.Nodes = nodes
	return list
}

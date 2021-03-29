/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * func.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

func (p *Parser) parseFuncOrMethodDecl() *ast.FuncDecl {
	fun := &ast.FuncDecl{Atom: p.curr}
	p.curr.Comment = nil
	if p.next() == token.LPAREN {
		fun.Recv = p.parseParams(token.PARAMS)
	}
	fun.Name = p.parseIdent()
	fun.Type = p.parseGenericSignature(p.pos())
	if p.tok() == token.SEMICOLON {
		// no function body
		p.next()
	} else {
		fun.Body = p.parseBlock()
	}
	return fun
}

func (p *Parser) parseFunctionLitOrType() ast.Node {
	pos := p.pos()
	node := p.parseFunctionType()
	if p.tok() != token.LBRACE {
		return node
	}
	binary := p.makeBinary()
	binary.Tok = token.LAMBDA
	binary.TokPos = pos
	binary.X = node
	binary.Y = p.parseBlock()
	return binary
}

func (p *Parser) parseFunctionType() *ast.FuncType {
	pos := p.pos()
	p.next() // skip 'func'
	return p.parseSignature(pos)
}

func (p *Parser) parseGenericSignature(funcpos token.Pos) ast.Node {
	if p.tok() != token.LBRACK {
		return p.parseSignature(funcpos)
	}
	gen := p.parseGenericParams(nil)
	gen.Type = p.parseSignature(funcpos)
	return gen
}

func (p *Parser) parseSignature(funcpos token.Pos) *ast.FuncType {
	return &ast.FuncType{
		Atom:    ast.Atom{Tok: token.FUNC, TokPos: funcpos, Comment: p.consumeComment()},
		Params:  p.parseParams(token.PARAMS),
		Results: p.parseResultOrNil(),
	}
}

func (p *Parser) parseParams(paramsOrResults token.Token) *ast.List {
	list := &ast.List{
		Atom: ast.Atom{Tok: paramsOrResults, TokPos: p.pos()},
	}
	nodes := p.enter(nil, token.LPAREN) // also skips '('
	if nodes != nil {
		list.Nodes = nodes
		return list
	}
	for !isLeave(p.tok()) {
		nodes = append(nodes, p.parseParamDecl())
		if p.tok() == token.COMMA {
			p.next() // skip ','
		} else {
			break
		}
	}
	nodes = p.leave(nodes, token.RPAREN)
	list.Nodes = p.fixParams(nodes)
	return list
}

func (p *Parser) parseParamDecl() *ast.Field {
	field := &ast.Field{
		Atom: ast.Atom{Tok: token.FIELD, TokPos: p.pos()},
	}
	var names []ast.Node
	for {
		if isLeave(p.tok()) {
			break
		}
		// we cannot use p.parseIdentList() here, because it may stop in the middle
		// of a qualified identifier or a generic type instantiation
		expr := p.parseTypeMaybeEllipsis()
		if expr.Op() != token.IDENT {
			field.Type = expr
			break
		}
		// expr is probably a name
		names = append(names, expr)
		if p.tok() != token.COMMA {
			break // token must belong to the type
		}
		p.next() // skip ','
	}
	if len(names) != 0 {
		field.Names = &ast.List{
			Atom:  ast.Atom{Tok: token.NAMES, TokPos: names[0].Pos()},
			Nodes: names,
		}
	}
	tok := p.tok()
	if tok != token.COMMA && !isLeave(tok) && field.Type == nil {
		field.Type = p.parseTypeMaybeEllipsis()
	}
	return field
}

func (p *Parser) parseResultOrNil() *ast.List {
	if tok := p.tok(); tok == token.LPAREN {
		// multiple results, listed inside "(" ")"
		return p.parseParams(token.RESULTS)
	} else if !isTypeStart(tok) {
		// no results
		return nil
	}
	// single result, without "(" ")"
	pos := p.pos()
	return &ast.List{
		Atom: ast.Atom{Tok: token.RESULTS, TokPos: pos},
		Nodes: []ast.Node{&ast.Field{
			Atom: ast.Atom{Tok: token.FIELD, TokPos: pos},
			Type: p.parseType(),
		}},
	}
}

// check the following cases:
// 1. if a field has no type => all names are actually types
// 2. cannot have both named and unnamed fields
// 3. also, only last field's type can start with '...'
func (p *Parser) fixParams(list []ast.Node) []ast.Node {
	n := len(list)
	if n == 0 {
		return list
	} else if n == 1 {
		field, ok := list[0].(*ast.Field)
		if !ok {
			// must be a Bad node - should not happen
			return list
		} else if field.Type == nil {
			return splitFieldNames(field)
		}
	}
	var haveNamed, haveUnnamed bool
	for i, node := range list {
		field, ok := node.(*ast.Field)
		if !ok {
			// must be a Bad node - should not happen
			return list
		}
		if field.Type == nil {
			// field has no type => its name (there must be only one!) is actually the type
			if field.Names != nil {
				names := field.Names.Nodes
				if len(names) == 1 {
					field.Type = names[0]
					field.Names = nil
					haveUnnamed = true
					continue
				}
			}
			list = append(list, p.makeBad(errParamsNamedUnnamed))
			return list
		}
		if field.Names != nil {
			haveNamed = true
		} else {
			haveUnnamed = true
		}
		if i+1 < n && field.Type.Op() == token.ELLIPSIS {
			// only last field's type can start with '...'
			field.Type = p.makeBadNode(field.Type, errParamNonFinalEllipsis)
			return list
		}
	}
	if haveNamed && haveUnnamed {
		list = append(list, p.makeBad(errParamsNamedUnnamed))
	}
	return list
}

// only a single field with one or more names but no type => all names are actually types
func splitFieldNames(field *ast.Field) []ast.Node {
	list := field.Names.Nodes
	field.Names = nil
	for i, elem := range list {
		pos := elem.Pos()
		list[i] = &ast.Field{
			Atom: ast.Atom{Tok: token.FIELD, TokPos: pos},
			Type: elem,
		}
	}
	return list
}

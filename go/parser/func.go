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
	if p.tok() == token.LBRACE {
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
	list.Nodes = p.fixParamDecls(nodes)
	return list
}

func (p *Parser) parseParamDecl() *ast.Field {
	field := &ast.Field{
		Atom: ast.Atom{Tok: token.FIELD, TokPos: p.pos()},
	}
	// we cannot use p.parseIdentList() here, because it may stop in the middle
	// of a qualified identifier or a generic type instantiation
	expr := p.parseTypeMaybeEllipsis()
	var name, typ ast.Node
	if isTypeStartOrEllipsis(p.tok()) {
		// identifier followed by its type
		name = expr
		typ = p.parseTypeMaybeEllipsis()
		if name.Op() != token.IDENT {
			name, typ = p.fixParamDecl(name, typ)
		}
	} else {
		// assume expr is a type. if needed, fixParams() will move it to a name
		typ = expr
	}
	// do NOT skip ',' because caller checks for it
	field.Type = typ
	if name != nil {
		field.Names = &ast.List{
			Atom:  ast.Atom{Tok: token.NAMES, TokPos: name.Pos()},
			Nodes: []ast.Node{name},
		}
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
	// single result, without '(' ')'
	pos := p.pos()
	return &ast.List{
		Atom: ast.Atom{Tok: token.RESULTS, TokPos: pos},
		Nodes: []ast.Node{&ast.Field{
			Atom: ast.Atom{Tok: token.FIELD, TokPos: pos},
			Type: p.parseType(),
		}},
	}
}

/**
 * there is an ambiguity parsing param declaration:
 * 'a[expr]b' should be parsed as name = 'a' and type = '[expr]b'
 * but expr may be arbitrarily complex => not enough lookahead
 * to distinguish from generic type instantiation 'a[expr],' or 'a[expr])'
 *
 * so it is currently parsed as a generic type instantiation
 * i.e. name = 'a[expr]' and type = 'b'
 *
 * fixParamDecl() fixes that
 */
func (p *Parser) fixParamDecl(name, typ ast.Node) (ast.Node, ast.Node) {
	if name.Op() == token.INDEX && name.Len() == 2 {
		index := name.At(1)
		name = name.At(0)
		typ = &ast.List{
			Atom:  ast.Atom{Tok: token.ARRAY, TokPos: index.Pos()},
			Nodes: []ast.Node{index, typ},
		}
	}
	if name.Op() != token.IDENT {
		name = p.makeBadNode(name, errExpectingIdent)
	}
	return name, typ
}

// parseParamDecl() above always stores a type in each field, and at most one name.
// if some fields have no name, we may need to change their type to a name
// and merge consecutive names.
func (p *Parser) fixParamDecls(list []ast.Node) []ast.Node {
	n := len(list)
	if n <= 1 {
		return list
	}
	var someHasName, lastHasName bool
	nameCount := 0

	for i, node := range list {
		field, ok := node.(*ast.Field)
		if !ok {
			// must be a Bad node - should not happen
			return list
		}
		if i+1 < n && field.Type.Op() == token.ELLIPSIS {
			// only last field's type can start with '...'
			field.Type = p.makeBadNode(field.Type, errParamNonFinalEllipsis)
		}
		if field.Names != nil {
			nameCount++
			if i < n-1 {
				someHasName = true
			} else {
				lastHasName = true
			}
		}
	}
	if nameCount == 0 || nameCount == n {
		// either all fields have names, or none has a name => nothing to do
		return list
	}
	if someHasName && !lastHasName {
		list = append(list, p.makeBad(errParamsNamedUnnamed))
		return list
	}
	var names []ast.Node
	dst := 0

	for _, node := range list {
		field, ok := node.(*ast.Field)
		if !ok {
			// checked above, should not happen
			continue
		}
		if field.Names == nil {
			// field has no name => its type is actually a name to be merged
			name := field.Type
			if name.Op() != token.IDENT {
				name = p.makeBadNode(name, errExpectingIdent)
			}
			names = append(names, name)
			continue
		}
		// field has both name and type => merge accumulated names into it
		if names != nil {
			list := field.Names
			list.Nodes = append(names, list.Nodes...)
			names = nil
		}
		list[dst] = field
		dst++
	}
	return list[0:dst]
}

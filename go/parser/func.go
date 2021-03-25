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
	fun.Type = p.parseSignature(p.pos())
	fun.Body = p.parseBlock()
	return fun
}

func (p *Parser) parseFunctionLit() *ast.Binary {
	binary := p.makeBinary()
	binary.Tok = token.LAMBDA
	binary.X = p.parseFunctionType()
	binary.Y = p.parseBlock()
	return binary
}

func (p *Parser) parseFunctionType() *ast.FuncType {
	pos := p.pos()
	p.next() // skip 'func'
	return p.parseSignature(pos)
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
	if p.tok() != token.LPAREN {
		list.Nodes = []ast.Node{p.parseBad(token.LPAREN.String())}
		return list
	}
	var nodes []ast.Node
	p.next() // skip '('
	all_ok := true
	for !isLeave(p.tok()) {
		field, ok := p.parseParamDecl()
		all_ok = all_ok && ok
		nodes = append(nodes, field)
		if p.tok() == token.COMMA {
			p.next() // skip ','
		} else {
			break
		}
	}
	if p.tok() == token.RPAREN {
		p.next() // skip ')'
	} else {
		nodes = append(nodes, p.makeBad(token.RPAREN.String()))
		all_ok = false
	}
	if all_ok {
		nodes = p.fixParams(nodes)
	}
	list.Nodes = nodes
	return list
}

func (p *Parser) parseParamDecl() (*ast.Field, bool) {
	ok := true
	field := &ast.Field{
		Atom: ast.Atom{Tok: token.FIELD, TokPos: p.pos()},
	}
	var head ast.Node
	if p.tok() == token.ELLIPSIS {
		field.Type = p.parseTypeMaybeEllipsis()
	} else {
		head = p.parseType()
	}
	if head != nil && p.tok() != token.COMMA && !isLeave(p.tok()) {
		// head is param name, it must be an unqualified identifier
		var name ast.Node
		if head.Op() == token.IDENT {
			name = head
		} else {
			name = p.makeBinaryBad(head, token.IDENT)
			ok = false
		}
		field.Names = &ast.List{
			Atom:  ast.Atom{Tok: token.NAMES, TokPos: name.Pos()},
			Nodes: []ast.Node{name},
		}
		head = nil // used already
	}
	if field.Type == nil {
		if head != nil {
			field.Type = head
		} else {
			field.Type = p.parseTypeMaybeEllipsis()
		}
	}
	return field, ok
}

func (p *Parser) parseResultOrNil() *ast.List {
	if tok := p.tok(); tok == token.LPAREN {
		// multiple results, listed inside "(" ")"
		return p.parseParams(token.RESULTS)
	} else if tok == token.SEMICOLON || tok == token.LBRACE || isLeave(tok) {
		// no results
		return nil
	}
	// single result, without "(" ")"
	field := &ast.Field{
		Atom: ast.Atom{Tok: token.FIELD, TokPos: p.pos()},
	}
	field.Type = p.parseType()
	return &ast.List{
		Atom:  ast.Atom{Tok: token.RESULTS, TokPos: field.TokPos},
		Nodes: []ast.Node{field},
	}
}

// if some fields have names and some don't, then merge unnamed fields
// (which actually contain a name, not a type) with the first following named field
func (p *Parser) fixParams(list []ast.Node) []ast.Node {
	n := len(list)
	if n <= 1 {
		return list
	}
	lastHasName := false
	othersNameCount := 0
	for i, node := range list {
		field, ok := node.(*ast.Field)
		if !ok {
			// must be a Bad node - should not happen
			return list
		}
		// only the last field can contain "..." in its type
		if i+1 < n && field.Type.Op() == token.ELLIPSIS {
			field.Type = p.makeBinaryBad(field.Type, errParamNonFinalEllipsis)
			return list
		}
		if field.Names == nil {
			continue
		} else if i == n-1 {
			lastHasName = true
		} else {
			othersNameCount++
		}
	}
	if !lastHasName {
		// last field is unnamed => others must be unnamed too
		if othersNameCount != 0 {
			list = append(list, p.makeBad(errParamsNamedUnnamed))
		}
	} else if othersNameCount != n-1 {
		// last fiels has name => merge the unnamed ones
		var names []ast.Node
		n = 0
		for _, node := range list {
			field := node.(*ast.Field)
			if field.Names == nil {
				// field.Type is actually the name: check that it's
				// an unqualified identifier, then save it in accumulated names
				name := field.Type
				if name.Op() != token.IDENT {
					name = p.makeBinaryBad(name, token.IDENT)
				}
				names = append(names, name)
				continue
			}
			// merge the accumulated names into the current field
			field.Names.Nodes = append(names, field.Names.Nodes...)
			names = nil
			list[n] = field
			n++
		}
		list = list[0:n]
	}
	if field := list[n-1].(*ast.Field); field.Type.Op() == token.ELLIPSIS {
		// if the last field contains "..." in its type
		// then it must have at most one name
		if names := field.Names; names != nil && len(names.Nodes) > 1 {
			field.Type = p.makeBinaryBad(field.Type, errParamNonFinalEllipsis)
		}
	}
	return list
}

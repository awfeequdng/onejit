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

func (p *Parser) parseFuncOrMethodDecl() (node ast.Node) {
	return nil // TODO
}

func (p *Parser) parseFunctionType() *ast.FuncType {
	return nil // TODO
}

func (p *Parser) parseSignature() *ast.FuncType {
	pos := p.pos()
	fun := &ast.FuncType{
		Atom: ast.Atom{Tok: token.FUNC, TokPos: pos},
		Params: &ast.List{
			Atom:  ast.Atom{Tok: token.PARAMS, TokPos: pos},
			Nodes: p.parseParams(),
		},
	}
	pos = p.pos()
	if results := p.parseResultOrNil(); results != nil {
		fun.Results = &ast.List{
			Atom:  ast.Atom{Tok: token.RESULTS, TokPos: pos},
			Nodes: p.parseParams(),
		}
	}
	return fun
}

func (p *Parser) parseParams() (list []ast.Node) {
	if p.tok() != token.LPAREN {
		return []ast.Node{p.parseBad(token.LPAREN.String())}
	}
	p.next() // skip '('
	for !isLeave(p.tok()) {
		list = append(list, p.parseParamDecl())
		if p.tok() == token.COMMA {
			p.next() // skip ','
		} else {
			break
		}
	}
	if p.tok() == token.RPAREN {
		p.next() // skip ')'
	} else {
		list = append(list, p.makeBad(token.RPAREN.String()))
	}
	// TODO if some fields have names and some don't,
	// the fields without names must be merged into the following field with name

	return list
}

func (p *Parser) parseParamDecl() *ast.Field {
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
		}
		field.Names = &ast.List{
			Atom:  ast.Atom{Tok: token.IDENTS, TokPos: name.Pos()},
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
	return field
}

func (p *Parser) parseResultOrNil() []ast.Node {
	if p.tok() == token.SEMICOLON || p.tok() == token.LBRACE || isLeave(p.tok()) {
		return nil
	}
	return nil // TODO
}

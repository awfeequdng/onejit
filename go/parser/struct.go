/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * struct.go
 *
 *  Created on: Mar 24, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

func (p *Parser) parseStructType() *ast.List {
	list := p.parseList() // skips 'struct'
	list.Nodes = p.parseFieldDeclList()
	return list
}

func (p *Parser) parseFieldDeclList() []ast.Node {
	list := p.enter(nil, token.LBRACE)
	if list != nil {
		return list
	}
	for !isLeave(p.tok()) {
		list = append(list, p.parseFieldDecl())

		if p.tok() == token.SEMICOLON {
			p.next() // skip ';'
		} else {
			break
		}
	}
	return p.leave(list, token.RBRACE)
}

/*
 * FieldDecl      = (IdentifierList Type | [ "*" ] TypeName) [ tag_string ] .
 * IdentifierList = identifier { "," identifier } .
 * TypeName       = identifier | QualifiedIdent .
 * QualifiedIdent = identifier "." identifier .
 *
 * the possible first tokens are:
 * "*"        => "*" TypeName
 * identifier => ambiguous, either IdentifierList or TypeName.
 *
 * after identifier, the possible second tokens are:
 * "." ";" "}" string => TypeName
 *
 */

func (p *Parser) parseFieldDecl() *ast.Field {
	field := &ast.Field{
		Atom: ast.Atom{Tok: token.FIELD, TokPos: p.pos()},
	}
	var names *ast.List
	if p.tok() == token.MUL {
		// embedded pointer type
		field.Type = p.parsePointerType()
	} else {
		// head is either the first name, or embedded (possibly qualified) type name
		head := p.parseQualifiedIdent()
		switch p.tok() {
		case token.SEMICOLON, token.RBRACE, token.STRING:
			// head is embedded (possibly qualified) type name
			field.Type = head
			head = nil
		case token.COMMA:
			// head is first name, parse the others
			p.next() // skip ','
			names = p.parseIdentList()
			names.Nodes = append([]ast.Node{head}, names.Nodes...)
		default:
			// head is the only name
			names = p.makeList()
			names.Nodes = []ast.Node{head}
		}
	}
	if names != nil {
		// first name must be an unqualified identifier
		if head := names.Nodes[0]; head.Op() != token.IDENT {
			names.Nodes[0] = p.makeBadNode(head, token.IDENT)
		}
		names.Tok = token.NAMES
		names.Lit = ""
		names.TokPos = names.Nodes[0].Pos()
		field.Names = names
	}
	if field.Type == nil {
		// no type yet, parse it
		field.Type = p.parseType()
	}
	if p.tok() == token.STRING {
		field.Tag = p.parseAtom(token.STRING)
	}
	return field
}

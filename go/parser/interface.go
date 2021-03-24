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

func (p *Parser) parseInterfaceType() *ast.List {
	list := p.parseList() // skips 'interface'
	list.Nodes = p.parseInterfaceBody()
	return list
}

func (p *Parser) parseInterfaceBody() (list []ast.Node) {
	if p.tok() != token.LBRACE {
		return []ast.Node{p.parseBad(token.LBRACE.String())}
	}
	p.next() // skip '{'
	for !isLeave(p.tok()) {
		list = append(list, p.parseInterfaceMethodOrEmbedded())

		if p.tok() == token.SEMICOLON {
			p.next() // skip ';'
		}
	}
	if p.tok() == token.RBRACE {
		p.next() // skip '}'
	} else {
		list = append(list, p.makeBad(token.RBRACE.String()))
	}
	return list
}

func (p *Parser) parseInterfaceMethodOrEmbedded() *ast.Field {
	field := &ast.Field{
		Atom: ast.Atom{Tok: token.FIELD, TokPos: p.pos()},
	}
	head := p.parseQualifiedIdent()
	if p.tok() != token.LPAREN {
		// head is embedded interface type name
		field.Type = head
	} else {
		// head is method name, it must be an unqualified ident
		if head.Op() != token.IDENT {
			head = p.makeBinaryBad(head, token.IDENT)
		}
		names := p.makeList()
		names.Nodes = []ast.Node{head}
		field.Type = p.parseSignature()
	}
	return field
}

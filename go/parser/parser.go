/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * parser.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/token"
)

type Parser struct {
	scanner *scanner.Scanner
	tok     token.Token
	lit     string
	pos     token.Pos
	end     token.Pos
	comment string
	err     []error
}

func (p *Parser) Init(s *scanner.Scanner) {
	*p = Parser{scanner: s}
}

func (p *Parser) Parse() (node ast.Node) {
	tok := p.next()
	switch tok {
	case token.PACKAGE:
		node = p.parsePackage()
	case token.IMPORT:
		node = p.parseImport()
	default:
		if isDecl(tok) {
			node = p.parseTopLevelDecl()
		} else {
			node = p.parseStmt()
		}
	}
	return node
}

func (p *Parser) next() token.Token {
	s := p.scanner
	for {
		p.tok, p.lit = s.Scan()
		p.pos, p.end = s.PosEnd()
		if p.tok != token.COMMENT {
			break
		}
		p.comment = p.lit
	}
	return p.tok
}

func (p *Parser) makeAtom() *ast.Atom {
	return &ast.Atom{Tok: p.tok, Lit: p.lit, TokPos: p.pos, TokEnd: p.end}
}

func (p *Parser) makeBad() *ast.Bad {
	return &ast.Bad{Tok: p.tok, Lit: p.lit, TokPos: p.pos, TokEnd: p.end}
}

func (p *Parser) makeSlice() *ast.Slice {
	return &ast.Slice{
		Atom: ast.Atom{Tok: p.tok, Lit: p.lit, TokPos: p.pos, TokEnd: p.end},
	}
}

func (p *Parser) parsePackage() ast.Node {
	ret := p.makeSlice()
	p.next()
	ret.Nodes = []ast.Node{p.makeIdent()}
	return ret
}

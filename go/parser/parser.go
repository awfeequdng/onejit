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

type Mode uint

const (
	TypeAlias Mode = 1 << iota // added in Go 1.9
	Generics                   // added in Go 1.xy
	Go1_9     = TypeAlias
	Default   = ^Mode(0)
)

type Parser struct {
	scanner *scanner.Scanner
	curr    ast.Atom
	mode    Mode
	errors  []*scanner.Error
}

func (p *Parser) Init(s *scanner.Scanner, mode Mode) {
	p.scanner = s
	p.curr = ast.Atom{}
	p.mode = mode
	p.errors = nil
}

// parse a single declaration, statement or expression
func (p *Parser) Parse() (node ast.Node) {
	tok := p.next()
	switch tok {
	case token.PACKAGE:
		node = p.parsePackage()
	case token.IMPORT:
		node = p.parseImport()
	default:
		if isDecl(tok) {
			node = p.ParseTopLevelDecl()
		} else {
			node = p.ParseStmt()
		}
	}
	return node
}

// get next non-comment token and store it in p.curr
func (p *Parser) next() token.Token {
	curr := &p.curr
	curr.Comment = nil
	s := p.scanner
	for {
		curr.Tok, curr.Lit = s.Scan()
		if curr.Tok != token.COMMENT {
			curr.TokPos, curr.TokEnd = s.PosEnd()
			break
		}
		curr.Comment = append(curr.Comment, curr.Lit)
	}
	return curr.Tok
}

func (p *Parser) tok() token.Token {
	return p.curr.Tok
}

func (p *Parser) pos() token.Pos {
	return p.curr.TokPos
}

func (p *Parser) consumeComment() []string {
	ret := p.curr.Comment
	p.curr.Comment = nil
	return ret
}

func (p *Parser) enter(list []ast.Node, tok token.Token) []ast.Node {
	if p.tok() == tok {
		p.next() // skip tok
	} else {
		list = append(list, p.makeBad(tok))
	}
	return list
}

// if current token != tok, report error and skip tokens
// until specified token is found
func (p *Parser) leave(list []ast.Node, tok token.Token) []ast.Node {
	for p.tok() != tok {
		list = append(list, p.parseBad(tok))
		if p.tok() == token.EOF {
			return list
		}
	}
	p.next() // skip tok
	return list
}

// if current token != tok, report error and skip tokens
// until specified token is found
func (p *Parser) leaveNode(node ast.Node, tok token.Token) ast.Node {
	var badnode bool
	for p.tok() != tok {
		if !badnode {
			badnode = true
			node = p.makeBadNode(node, tok)
		}
		if p.tok() == token.EOF {
			return node
		}
		p.next()
	}
	p.next() // skip tok
	return node
}

func (p *Parser) makeAtom(tok token.Token) *ast.Atom {
	atom := p.curr
	atom.Tok = tok
	p.curr.Comment = nil
	return &atom
}

func (p *Parser) makeBad(msg interface{}) *ast.Bad {
	err := p.error(msg)
	tok := p.tok()
	bad := &ast.Bad{
		Atom: ast.Atom{Tok: tok, TokPos: p.pos()},
		Node: p.makeAtom(tok),
		Err:  err,
	}
	p.curr.Comment = nil
	return bad
}

func (p *Parser) makeBadNode(x ast.Node, msg interface{}) *ast.Bad {
	bad := p.makeBad(msg)
	if tok, ok := msg.(token.Token); ok {
		bad.Tok = tok
	} else {
		bad.Tok = x.Op()
	}
	bad.TokPos = x.Pos()
	bad.Node = x
	return bad
}

func (p *Parser) makeBinary() (binary *ast.Binary) {
	binary = &ast.Binary{Atom: p.curr}
	p.curr.Comment = nil
	return binary
}

func (p *Parser) makeIdent() (node ast.Node) {
	if p.tok() == token.IDENT {
		node = p.makeAtom(token.IDENT)
	} else {
		node = p.makeBad(errExpectingIdent)
	}
	p.curr.Comment = nil
	return node
}

func (p *Parser) makeList() (list *ast.List) {
	list = &ast.List{Atom: p.curr}
	p.curr.Comment = nil
	return list
}

func (p *Parser) makeUnary() (unary *ast.Unary) {
	unary = &ast.Unary{Atom: p.curr}
	p.curr.Comment = nil
	return unary
}

func (p *Parser) parseAtom(tok token.Token) *ast.Atom {
	node := p.makeAtom(tok)
	p.next()
	return node
}

func (p *Parser) parseBad(msg interface{}) *ast.Bad {
	node := p.makeBad(msg)
	p.next()
	return node
}

func (p *Parser) parseBinary() *ast.Binary {
	node := p.makeBinary()
	p.next()
	return node
}

func (p *Parser) parseIdent() ast.Node {
	node := p.makeIdent()
	p.next()
	return node
}

func (p *Parser) parseList() *ast.List {
	node := p.makeList()
	p.next()
	return node
}

func (p *Parser) parseUnary() *ast.Unary {
	node := p.makeUnary()
	p.next()
	return node
}

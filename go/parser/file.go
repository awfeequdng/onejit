/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * file.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

// parse a whole file.
func (p *Parser) ParseFile() *ast.File {
	file := ast.File{Atom: ast.Atom{Tok: token.FILE}}

	var imports []ast.Node
	var decls []ast.Node
	var eof bool
	for !eof {
		switch tok := p.tok(); tok {
		case token.EOF:
			eof = true
		case token.PACKAGE:
			if imports != nil || decls != nil {
				p.error(p.pos(), errExpectingDecl)
			} else if file.Package != nil {
				p.error(p.pos(), errDuplicatePackage)
			}
			node := p.parsePackage()
			if file.Package == nil {
				file.Package = node
			}
			eof = p.Mode&(ParseImports|ParseDecls) == 0
		case token.IMPORT:
			if p.Mode&(ParseImports|ParseDecls) == 0 {
				eof = true
				continue
			}
			if decls != nil {
				p.error(p.pos(), errExpectingDecl)
			}
			node := p.parseImport()
			if p.Mode&ParseImports != 0 {
				imports = append(imports, node)
			}
		case token.SEMICOLON:
			p.next()
			continue
		default:
			if p.Mode&ParseDecls == 0 {
				eof = true
				continue
			}
			var node ast.Node
			if isDecl(tok) {
				node = p.ParseTopLevelDecl()
			} else {
				p.error(p.pos(), errExpectingDecl)
				node = p.parseStmt(allowCompositeLit)
			}
			decls = append(decls, node)
		}
	}
	file.Imports = makeList(token.IMPORTS, imports)
	file.Decls = makeList(token.DECLS, decls)
	return &file
}

func makeList(tok token.Token, nodes []ast.Node) *ast.List {
	if len(nodes) == 0 {
		return nil
	}
	return &ast.List{
		Atom:  ast.Atom{Tok: tok, TokPos: nodes[0].Pos()},
		Nodes: nodes,
	}
}

func (p *Parser) parsePackage() *ast.Unary {
	node := p.parseUnary()
	if tok := p.tok(); tok == token.IDENT || tok == token.STRING {
		node.X = p.parseAtom(tok)
	} else {
		node.X = p.parseBad(errExpectingIdentOrString)
	}
	return node
}

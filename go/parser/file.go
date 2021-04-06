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

func (p *Parser) ParseFile() *ast.File {
	file := ast.File{Atom: ast.Atom{Tok: token.FILE}}

	var imports []ast.Node
	var decls []ast.Node
	for {
		node := p.Parse()
		if node == nil {
			continue
		} else if op := node.Op(); op == token.ILLEGAL {
			continue
		} else if op == token.EOF {
			break // end of file
		} else if op == token.PACKAGE {
			if file.Package != nil {
				p.error(node.Pos(), errDuplicatePackage)
				continue
			}
			file.Package = node.(*ast.Unary)
			if imports != nil || decls != nil {
				p.error(node.Pos(), errExpectingDecl)
			}
		} else if op == token.IMPORT {
			if decls != nil {
				p.error(node.Pos(), errExpectingDecl)
			}
			imports = append(imports, node)
		} else {
			if !isDecl(op) {
				p.error(node.Pos(), errExpectingDecl)
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
	node.X = p.parseIdent()
	return node
}

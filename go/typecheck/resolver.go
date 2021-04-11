/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * resolver.go
 *
 *  Created on: Apr 10, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

// resolves symbols i.e. for each identifier finds its declaration
type Resolver struct {
	multiscope
	resolved map[ast.Node]*Symbol
}

// does NOT clear accumulated errors
func (r *Resolver) Init(c *Collector) {
	r.multiscope = c.multiscope
	r.resolved = nil
}

func (r *Resolver) Globals(nodes ...ast.Node) {
	for _, node := range nodes {
		if node == nil {
			continue
		}
		switch op := node.Op(); op {
		case token.DIR:
			r.fileset = node.(*ast.Dir).FileSet
			r.globalList(node)
		case token.FILE:
			r.curr = node.(*ast.File).File
			r.globalList(node)
		case token.IMPORTS, token.DECLS:
			r.globalList(node)
		case token.FUNC:
			r.funcDecl(node)
		case token.IMPORT:
			// nothing to do
		case token.CONST, token.TYPE, token.VAR:
			r.node(node)
		}
	}
}

func (r *Resolver) globalList(list ast.Node) {
	for i, n := 0, list.Len(); i < n; i++ {
		r.Globals(list.At(i))
	}
}

func (r *Resolver) funcDecl(decl ast.Node) {
	recv := decl.At(0)
	r.node(recv)

	name := decl.At(1).(*ast.Atom)
	if recv == nil && name.Lit != "init" {
		// TODO methods
		r.ident(name)
	}
	r.node(decl.At(2)) // type

	// TODO: resolve function body.
	// It contains local symbols (declared before use)
	// and labels (may be declared *after* use)
}

func (r *Resolver) typeSpec(spec ast.Node) {
	op := spec.Op()
	if op != token.ASSIGN && op != token.DEFINE {
		r.error(spec, "invalid type declaration: "+spec.String())
		return
	}
	r.node(spec.At(0)) // name
	r.node(spec.At(1)) // type
}

func (r *Resolver) node(node ast.Node) {
	if node == nil {
		return
	}
	if node.Op() == token.IDENT {
		r.ident(node.(*ast.Atom))
		return
	}
	for i, n := 0, node.Len(); i < n; i++ {
		r.node(node.At(i))
	}
}

func (r *Resolver) ident(node ast.Node) {
	name := node.(*ast.Atom).Lit
	sym := r.lookup(name)
	if sym == nil {
		r.error(node, "undefined: "+name)
		return
	}
	if r.resolved == nil {
		r.resolved = make(map[ast.Node]*Symbol)
	}
	r.resolved[node] = sym
}

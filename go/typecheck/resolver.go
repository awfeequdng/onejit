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
	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/token"
)

// resolves symbols i.e. for each declared identifier finds its declaration
// also computes the dependencies among declared identifiers
type Resolver struct {
	multiscope
	resolved map[ast.Node]*Symbol
	depfwd   SymbolGraph
	depinv   SymbolGraph
	sources  []ast.Node
}

// does NOT clear accumulated errors
func (r *Resolver) Init(c *Collector) {
	r.multiscope = c.multiscope
	r.resolved = nil
	r.depfwd = nil
	r.depinv = nil
	r.sources = c.sources
}

// resolve global symbols collected by Collector
func (r *Resolver) Globals() {
	for _, node := range r.sources {
		r.global(node)
	}
}

func (r *Resolver) global(node ast.Node) {
	if node == nil {
		return
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
	case token.TYPE:
		for i, n := 0, node.Len(); i < n; i++ {
			r.typeSpec(node.At(i))
		}
	case token.CONST, token.VAR:
		for i, n := 0, node.Len(); i < n; i++ {
			r.valueSpec(node.At(i))
		}
	}
}

func (r *Resolver) globalList(list ast.Node) {
	for i, n := 0, list.Len(); i < n; i++ {
		r.global(list.At(i))
	}
}

func (r *Resolver) funcDecl(decl ast.Node) {
	name := decl.At(1).(*ast.Atom)
	if decl.At(0) /*recv*/ != nil || name.Lit == "init" {
		// TODO methods
		return
	}
	sym := r.ident(nil, name)
	r.node([]*Symbol{sym}, decl.At(2)) // type

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
	sym := r.ident(nil, spec.At(0))
	if sym == nil {
		return
	}
	r.node([]*Symbol{sym}, spec.At(1)) // type
}

func (r *Resolver) valueSpec(spec ast.Node) {
	names := spec.At(0)
	n := names.Len()

	idents := make([]*Symbol, n)
	for i := 0; i < n; i++ {
		sym := r.ident(nil, names.At(i))
		if sym == nil {
			return
		}
		idents[i] = sym
	}

	r.node(idents, spec.At(1)) // type

	init := spec.At(2) // init.Op() == token.EXPRS
	if init == nil {
		return
	} else if init.Len() == n {
		// one initializer per const or var
		for i := 0; i < n; i++ {
			r.node(idents[i:i+1], init.At(i))
		}
	} else if init.Len() == 1 {
		r.node(idents, init)
	} else {
		r.error(spec, "expecting either 1 or "+strings.IntToString(n)+" initialize expressions")
	}
}

func (r *Resolver) node(curr []*Symbol, node ast.Node) {
	if node == nil {
		return
	}
	if node.Op() == token.IDENT {
		r.ident(curr, node.(*ast.Atom))
		return
	}
	i, n := 0, node.Len()
	if node.Op() == token.PERIOD {
		// TODO resolve qualified identifiers
		n = 1
	}
	for ; i < n; i++ {
		r.node(curr, node.At(i))
	}
}

func (r *Resolver) ident(curr []*Symbol, node ast.Node) *Symbol {
	sym, imported := r.resolve(node)
	if !imported {
		r.deps(curr, sym)
	}
	return sym
}

func (r *Resolver) resolve(node ast.Node) (sym *Symbol, imported bool) {
	ident := node.(*ast.Atom)
	name := ident.Lit
	sym, imported = r.lookup(name)
	if sym == nil {
		r.error(node, "undefined: "+name)
	} else if !imported {
		if r.resolved == nil {
			r.resolved = make(map[ast.Node]*Symbol)
		}
		r.resolved[node] = sym
	}
	return sym, imported
}

// add dependencies fromlist -> to
func (r *Resolver) deps(fromlist []*Symbol, to *Symbol) {
	if len(fromlist) == 0 || to == nil {
		return
	}
	if r.depfwd == nil {
		r.depfwd = make(SymbolGraph)
	}
	if r.depinv == nil {
		r.depinv = make(SymbolGraph)
	}
	for _, from := range fromlist {
		r.depfwd.Link(from, to)
		r.depinv.Link(to, from)
	}
}

// create types.Type for all global declarations passed to Globals()
func (r *Resolver) DeclareGlobals() {
	m := dup(r.syms)
	for len(m) != 0 {
		sym := r.pickSymbolNoDeps(m)
		if sym == nil {
			break
		}
		r.declare(sym)
		r.removeDeps(sym)
		delete(m, sym.Name())
	}
}

func dup(m SymbolMap) SymbolMap {
	ret := make(SymbolMap, len(m))
	for name, sym := range m {
		ret[name] = sym
	}
	return ret
}

func (r *Resolver) pickSymbolNoDeps(m SymbolMap) *Symbol {
	// fmt.Println("depfwd: ", r.depfwd)
	// fmt.Println("depinv: ", r.depinv)
	for _, sym := range m {
		if len(r.depfwd[sym]) == 0 {
			// fmt.Println("picked: ", sym.String())
			return sym
		}
	}
	return nil
}

func (r *Resolver) declare(sym *Symbol) {
	// fmt.Println("declare:", sym.String())
}

func (r *Resolver) removeDeps(sym *Symbol) {
	for fwd := range r.depinv[sym] {
		dep := r.depfwd[fwd]
		delete(dep, sym)
		if len(dep) == 0 {
			delete(r.depfwd, fwd)
		}
	}
	delete(r.depinv, sym)
}

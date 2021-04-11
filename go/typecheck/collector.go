/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * collector.go
 *
 *  Created on: Apr 06, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

// collects symbols defined in ast.Node
type Collector struct {
	multiscope
	initfuncs []ast.Node     // list of global func init() { ... }
	knownpkgs types.Packages // list of known packages
	sources   []ast.Node
}

var typeAlias ast.Node = &ast.Atom{Tok: token.ASSIGN}

// does NOT clear accumulated errors and warnings
func (c *Collector) Init(fileset *token.FileSet, scope *types.Scope, knownpkgs types.Packages) {
	c.multiscope.Init(fileset, scope)
	c.initfuncs = nil
	c.knownpkgs = knownpkgs
}

// collect global declarations from specified nodes.
func (c *Collector) Globals(nodes ...ast.Node) {
	c.sources = nodes
	for _, node := range nodes {
		c.global(node)
	}
}

func (c *Collector) global(node ast.Node) {
	if node == nil {
		return
	}
	switch op := node.Op(); op {
	case token.DIR:
		c.fileset = node.(*ast.Dir).FileSet
		c.globalList(node)
	case token.FILE:
		c.curr = node.(*ast.File).File
		c.globalList(node)
	case token.IMPORTS, token.DECLS:
		c.globalList(node)
	case token.FUNC:
		c.funcDecl(node)
	case token.IMPORT:
		for i, n := 0, node.Len(); i < n; i++ {
			c.importSpec(node.At(i))
		}
	case token.TYPE:
		for i, n := 0, node.Len(); i < n; i++ {
			c.typeSpec(node.At(i))
		}
	case token.VAR, token.CONST:
		for i, n := 0, node.Len(); i < n; i++ {
			c.valueSpec(op, node.At(i))
		}
	}
}

func (c *Collector) globalList(list ast.Node) {
	for i, n := 0, list.Len(); i < n; i++ {
		c.global(list.At(i))
	}
}

func (c *Collector) funcDecl(decl ast.Node) {
	if recv := decl.At(0); recv != nil {
		return // TODO collectMethodDecl
	}
	name := decl.At(1).(*ast.Atom)
	typ := decl.At(2)
	body := decl.At(3)
	if name.Lit == "init" {
		c.initfuncs = append(c.initfuncs, decl)
		if typ.Op() != token.FUNC || typ.At(0).Len() != 0 || typ.At(1) != nil {
			c.error(decl, "func init must have no arguments and no return values")
		}
	} else {
		c.multiscope.add(decl, types.FuncObj, name.Lit, typ, body, NoIndex)
	}
}

func (c *Collector) importSpec(spec ast.Node) {
	op := spec.Op()
	if op != token.IMPORT_SPEC {
		c.error(spec, "invalid import declaration: "+spec.String())
		return
	}
	name := spec.At(0)
	path := spec.At(1).(*ast.Atom)
	// also skip initial and final '"'
	namestr, pathstr := "", strings.Unescape(path.Lit[1:len(path.Lit)-1])

	pkg := c.knownpkgs[pathstr]
	if pkg == nil {
		c.error(spec, "package not known: "+pathstr)
	}
	if name != nil {
		if name.Op() == token.PERIOD {
			namestr = "." // import . "some/pkg/path"
		} else {
			namestr = name.(*ast.Atom).Lit // import foo "some/pkg/path"
		}
	} else if pkg != nil {
		namestr = pkg.Name() // import "some/pkg/path"
	} else {
		namestr = strings.Basename(pathstr) // approximate
	}
	c.addImport(spec, namestr, pkg)
}

func (c *Collector) typeSpec(spec ast.Node) {
	op := spec.Op()
	if op != token.ASSIGN && op != token.DEFINE {
		c.error(spec, "invalid type declaration: "+spec.String())
		return
	}
	name := spec.At(0).(*ast.Atom)
	typ := spec.At(1)
	var init ast.Node
	if op == token.ASSIGN {
		init = typeAlias
	}
	c.add(spec, types.TypeObj, name.Lit, typ, init, NoIndex)
}

func (c *Collector) valueSpec(op token.Token, spec ast.Node) {
	if spec.Op() != token.VALUE_SPEC {
		c.error(spec, "invalid "+op.String()+" declaration: "+spec.String())
		return
	}
	names := spec.At(0)
	n := names.Len()
	typ := spec.At(1)
	init := spec.At(2) // init.Op() == token.EXPRS
	oneInitializerPerName := false
	if init != nil {
		switch ninit := init.Len(); ninit {
		case n:
			oneInitializerPerName = true
		case 1:
			init = init.At(0)
		case 0:
			init = nil
		default:
			c.error(spec, "invalid "+op.String()+" declaration, found "+
				strings.IntToString(ninit)+" initializers"+
				", expecting 0, 1 or "+strings.IntToString(n)+
				": "+spec.String())
			return
		}
	}

	cls := token2class(op)
	for i := 0; i < n; i++ {
		atom := names.At(i).(*ast.Atom)
		if oneInitializerPerName {
			c.add(spec, cls, atom.Lit, typ, init.At(i), NoIndex)
		} else {
			c.add(spec, cls, atom.Lit, typ, init, i)
		}
	}
}

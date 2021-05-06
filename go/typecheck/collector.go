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
	knownpkgs types.Packages // list of known packages
	sources   []ast.Node
}

var typeAlias ast.Node = &ast.Atom{Tok: token.ASSIGN}

// does NOT clear accumulated errors and warnings
func (c *Collector) Init(fileset *token.FileSet, scope *types.Scope, knownpkgs types.Packages) {
	c.multiscope.Init(fileset, scope)
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
	var list ast.Node
	switch op := node.Op(); op {
	case token.DIR:
		c.fileset = node.(*ast.Dir).FileSet
		list = node
	case token.FILE:
		c.currfile = node.(*ast.File).File
		list = node
	case token.IMPORTS, token.DECLS:
		list = node
	case token.FUNC:
		c.funcDecl(node)
	case token.IMPORT:
		for i, n := 0, node.Len(); i < n; i++ {
			c.importSpec(node.At(i))
		}
	case token.CONST:
		var lastType, lastInit ast.Node
		for i, n := 0, node.Len(); i < n; i++ {
			lastType, lastInit = c.constSpec(node.At(i), lastType, lastInit, i)
		}
	case token.TYPE:
		for i, n := 0, node.Len(); i < n; i++ {
			c.typeSpec(node.At(i))
		}
	case token.VAR:
		for i, n := 0, node.Len(); i < n; i++ {
			c.varSpec(node.At(i))
		}
	}
	if list != nil {
		for i, n := 0, list.Len(); i < n; i++ {
			c.global(list.At(i))
		}
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
		c.multiscope.addInitFunc(decl, typ, body)
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
		c.error(spec, `package "`+pathstr+`" not loaded`)
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

// collect a const declaration.
// for const declarations, use lastType and lastInit if neither type or init are present
// return updated lastType and lastInit
func (c *Collector) valueSpec(op token.Token, spec ast.Node, lastType ast.Node, lastInit ast.Node, iotaVal int) (ast.Node, ast.Node) {
	if spec.Op() != token.VALUE_SPEC {
		c.error(spec, "invalid "+op.String()+" declaration: "+spec.String())
		return lastType, lastInit
	}
	names := spec.At(0)
	n := names.Len()
	typ := spec.At(1)
	init := spec.At(2) // init.Op() == token.EXPRS
	if op == token.CONST {
		if typ == nil && init == nil {
			// if type and init are missing, reuse last ones
			typ = lastType
			init = lastInit
		} else {
			// if either type or init are present, allow later reuse
			lastType = typ
			lastInit = init
		}
	}
	oneInitializerPerName := false
	if init != nil {
		switch ninit := init.Len(); ninit {
		case n:
			oneInitializerPerName = true
		case 1:
			if op == token.CONST {
				c.error(spec, "invalid const declaration, found 1 initializer, expecting 0 or "+
					strings.IntToString(n)+": "+spec.String())
			}
			init = init.At(0)
		case 0:
			init = nil
		default:
			c.error(spec, "invalid "+op.String()+" declaration, found "+
				strings.IntToString(ninit)+" initializers"+
				", expecting 0, 1 or "+strings.IntToString(n)+
				": "+spec.String())
			return lastType, lastInit
		}
	}

	cls := token2class(op)
	for i := 0; i < n; i++ {
		atom := names.At(i).(*ast.Atom)
		if op == token.CONST {
			c.add(spec, cls, atom.Lit, typ, init.At(i), iotaVal)
		} else if oneInitializerPerName {
			c.add(spec, cls, atom.Lit, typ, init.At(i), NoIndex)
		} else {
			c.add(spec, cls, atom.Lit, typ, init, i)
		}
	}
	return lastType, lastInit
}

// collect a const declaration.
// use lastType and lastInit if neither type or init are present.
// return updated lastType and lastInit
func (c *Collector) constSpec(spec ast.Node, lastType ast.Node, lastInit ast.Node, iotaVal int) (ast.Node, ast.Node) {
	if spec.Op() != token.VALUE_SPEC {
		c.error(spec, "invalid const declaration: "+spec.String())
		return lastType, lastInit
	}
	names := spec.At(0)
	n := names.Len()
	typ := spec.At(1)
	init := spec.At(2) // init.Op() == token.EXPRS
	if typ == nil && init == nil {
		// if type and init are missing, reuse last ones
		typ = lastType
		init = lastInit
	} else {
		// if either type or init are present, allow later reuse
		lastType = typ
		lastInit = init
	}
	if init == nil && typ == nil {
		c.error(spec, "missing type or value in const declaration: "+spec.String())
		return lastType, lastInit
	}

	if init != nil {
		switch ninit := init.Len(); ninit {
		case n:
			break
		case 1:
			c.error(spec, "invalid const declaration, found 1 initializer, expecting 0 or "+
				strings.IntToString(n)+": "+spec.String())
			init = init.At(0)
		case 0:
			init = nil
		default:
			c.error(spec, "invalid const declaration, found "+
				strings.IntToString(ninit)+" initializers"+
				", expecting 0 or "+strings.IntToString(n)+
				": "+spec.String())
			return lastType, lastInit
		}
	}

	for i := 0; i < n; i++ {
		atom := names.At(i).(*ast.Atom)
		c.add(spec, types.ConstObj, atom.Lit, typ, init.At(i), iotaVal)
	}
	return lastType, lastInit
}

// collect a var declaration.
func (c *Collector) varSpec(spec ast.Node) {
	if spec.Op() != token.VALUE_SPEC {
		c.error(spec, "invalid var declaration: "+spec.String())
		return
	}
	names := spec.At(0)
	n := names.Len()
	typ := spec.At(1)
	init := spec.At(2) // init.Op() == token.EXPRS
	oneInitializerPerName := false

	if init == nil && typ == nil {
		c.error(spec, "missing type or initializer in var declaration: "+spec.String())
		return
	}
	if init != nil {
		switch ninit := init.Len(); ninit {
		case n:
			oneInitializerPerName = true
		case 1:
			init = init.At(0)
		case 0:
			init = nil
		default:
			c.error(spec, "invalid var declaration, found "+
				strings.IntToString(ninit)+" initializers"+
				", expecting 0, 1 or "+strings.IntToString(n)+
				": "+spec.String())
			return
		}
	}

	var init_i ast.Node
	var index_i int
	for i := 0; i < n; i++ {
		atom := names.At(i).(*ast.Atom)
		if oneInitializerPerName {
			init_i, index_i = init.At(i), NoIndex
		} else {
			init_i, index_i = init, i
		}
		c.add(spec, types.VarObj, atom.Lit, typ, init_i, index_i)
	}
}

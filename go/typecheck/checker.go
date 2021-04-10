/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * checker.go
 *
 *  Created on: Apr 06, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

type Checker struct {
	scope     *types.Scope                 // existing scope being extended
	objs      *types.Scope                 // declared symbols
	fileobjs  map[*token.File]*types.Scope // per-file symbols: imports and dot imports
	initfuncs []ast.Node                   // list of global func init() { ... }
	typemap   TypeMap
	knownpkgs types.Packages // list of known packages
	fileset   *token.FileSet
	file      *token.File // current file being processed. may be nil
	warnings  []*scanner.Error
	errors    []*scanner.Error
}

const noIndex int = -1

var typeAlias ast.Node = &ast.Atom{Tok: token.ASSIGN}

// does NOT clear accumulated errors and warnings
func (c *Checker) Init(fileset *token.FileSet, scope *types.Scope, knownpkgs types.Packages) {
	c.scope = scope
	c.objs = nil
	c.fileobjs = nil
	c.initfuncs = nil
	c.typemap = nil
	c.knownpkgs = knownpkgs
	c.fileset = fileset
	c.file = nil
}

func (c *Checker) CheckDir(dir *ast.Dir) {
	c.CheckFiles(dir.Files...)
}

func (c *Checker) CheckFiles(files ...*ast.File) {
	nodes := make([]ast.Node, len(files))
	for i, file := range files {
		nodes[i] = file
	}
	c.CheckGlobals(nodes...)
}

func (c *Checker) CheckGlobals(source ...ast.Node) {
	c.collectGlobals(source...)
}

func (c *Checker) collectGlobals(nodes ...ast.Node) {
	for _, node := range nodes {
		if node == nil {
			continue
		}
		switch op := node.Op(); op {
		case token.DIR, token.FILE, token.IMPORTS, token.DECLS:
			for i, n := 0, node.Len(); i < n; i++ {
				c.collectGlobals(node.At(i))
			}
		case token.FUNC:
			c.collectFuncDecl(node)
		case token.IMPORT:
			for i, n := 0, node.Len(); i < n; i++ {
				c.collectImportSpec(node.At(i))
			}
		case token.TYPE:
			for i, n := 0, node.Len(); i < n; i++ {
				c.collectTypeSpec(node.At(i))
			}
		case token.VAR, token.CONST:
			for i, n := 0, node.Len(); i < n; i++ {
				c.collectValueSpec(op, node.At(i))
			}
		}
	}
}

func (c *Checker) collectFuncDecl(decl ast.Node) {
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
		c.addDecl(decl, types.FuncObj, name.Lit, typ, body, noIndex)
	}
}

func (c *Checker) collectImportSpec(spec ast.Node) {
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
	c.addImportDecl(spec, namestr, pkg)
}

func (c *Checker) collectTypeSpec(spec ast.Node) {
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
	c.addDecl(spec, types.TypeObj, name.Lit, typ, init, noIndex)
}

func (c *Checker) collectValueSpec(op token.Token, spec ast.Node) {
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
			c.addDecl(spec, cls, atom.Lit, typ, init.At(i), noIndex)
		} else {
			c.addDecl(spec, cls, atom.Lit, typ, init, i)
		}
	}
}

func (c *Checker) addDecl(node ast.Node, cls types.Class, name string, typ ast.Node, init ast.Node, index int) {
	if name == "_" {
		return
	}
	d := &decl{node: node, typ: typ, init: init, index: index}
	c.addObjDecl(cls, name, d)
}

func (c *Checker) addImportDecl(node ast.Node, name string, pkg *types.Package) {
	if name == "_" {
		return
	}
	if name != "." {
		d := &decl{node: node, value: pkg}
		c.addFileDecl(types.ImportObj, name, d)
		return
	}
	// dot import
	if pkg == nil {
		return
	}
	if c.objs == nil {
		c.objs = types.NewScope(c.scope.Parent())
	}
	scope := pkg.Scope()
	for _, name := range scope.Names() {
		if !token.IsExported(name) {
			continue
		}
		obj := scope.Lookup(name)
		if obj.Class() == types.ImportObj {
			continue // should not happen, imports are added to a nested scope
		}
		c.checkRedefined(name, node)
		c.objs.Insert(obj) // reuse the same object
	}
}

func (c *Checker) addObjDecl(cls types.Class, name string, d *decl) {
	c.checkRedefined(name, d.node)
	obj := types.NewObject(cls, name, nil)
	obj.SetValue(d)
	if c.objs == nil {
		c.objs = types.NewScope(c.scope.Parent())
	}
	c.objs.Insert(obj)
}

func (c *Checker) addFileDecl(cls types.Class, name string, d *decl) {
	c.checkRedefined(name, d.node)
	obj := types.NewObject(cls, name, nil)
	obj.SetValue(d)
	if c.fileobjs == nil {
		c.fileobjs = make(map[*token.File]*types.Scope)
	}
	fileobjs := c.fileobjs[c.file]
	if fileobjs == nil {
		if c.objs == nil {
			c.objs = types.NewScope(c.scope.Parent())
		}
		fileobjs = types.NewScope(c.objs)
		c.fileobjs[c.file] = fileobjs
	}
	fileobjs.Insert(obj)
}

func (c *Checker) checkRedefined(name string, node ast.Node) {
	if old := c.objs.Lookup(name); old != nil {
		c.errorRedefined(name, node, old)
	} else if old := c.fileobjs[c.file].Lookup(name); old != nil {
		c.errorRedefined(name, node, old)
	} else if c.scope.Lookup(name) != nil {
		c.warning(node, name+" redeclared in this block")
	}
}

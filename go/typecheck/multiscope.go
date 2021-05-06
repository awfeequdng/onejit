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
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

// symbols i.e. declarations plus per-file symbols i.e. imports and dot imports
type multiscope struct {
	outer     *types.Scope              // existing scope being extended
	named     ObjectMap                 // declared objects, indexed by name
	unnamed   []*Object                 // declared _ objects
	initfuncs []*Object                 // list of global func init() { ... }
	fileobjs  map[*token.File]ObjectMap // per-file objects: imports and dot imports
	currfile  *token.File               // current file
	universe  *types.Scope
	errors
}

// does NOT clear accumulated errors
func (ms *multiscope) Init(fileset *token.FileSet, outer *types.Scope) {
	ms.outer = outer
	ms.named = nil
	ms.unnamed = nil
	ms.initfuncs = nil
	ms.fileobjs = nil
	ms.currfile = nil
	for outer.Parent() != nil {
		outer = outer.Parent()
	}
	ms.universe = outer
	ms.errors.fileset = fileset
}

func (ms *multiscope) lookup(name string) (obj *Object, imported bool) {
	obj, imported = ms.fileobjs[ms.currfile][name]
	if obj == nil {
		obj = ms.named[name]
		imported = false
	}
	return obj, imported
}

func (ms *multiscope) lookupParent(name string) (obj *Object, imported bool) {
	obj, imported = ms.lookup(name)
	if obj == nil {
		_, tobj := ms.outer.LookupParent(name)
		if tobj != nil {
			obj = (*Object)(tobj)
			imported = true
		}
	}
	return obj, imported
}

func (ms *multiscope) getFile() ObjectMap {
	file := ms.fileobjs[ms.currfile]
	if file == nil {
		file = ms.getFileSlow()
	}
	return file
}

func (ms *multiscope) getFileSlow() ObjectMap {
	file := make(ObjectMap)
	files := ms.fileobjs
	if files == nil {
		files = make(map[*token.File]ObjectMap)
		ms.fileobjs = files
	}
	ms.fileobjs[ms.currfile] = file
	return file
}

func (ms *multiscope) getSyms() ObjectMap {
	syms := ms.named
	if syms == nil {
		syms = make(ObjectMap)
		ms.named = syms
	}
	return syms
}

func (ms *multiscope) newObject(node ast.Node, cls types.Class, name string, typ ast.Node, init ast.Node, index int) *Object {
	obj := NewObject(cls, name, node, ms.currfile)
	decl := obj.Decl()
	decl.typ = typ
	decl.init = init
	decl.index = index
	return obj
}

func (ms *multiscope) add(node ast.Node, cls types.Class, name string, typ ast.Node, init ast.Node, index int) {
	obj := ms.newObject(node, cls, name, typ, init, index)
	if name != "_" {
		ms.checkRedefined(name, node)
		ms.getSyms().Insert(obj)
	} else {
		ms.unnamed = append(ms.unnamed, obj)
	}
}

func (ms *multiscope) addInitFunc(decl ast.Node, typ ast.Node, body ast.Node) {
	obj := ms.newObject(decl, types.FuncObj, "init", typ, body, NoIndex)
	ms.initfuncs = append(ms.initfuncs, obj)
	if typ.Op() != token.FUNC || typ.At(0).Len() != 0 || typ.At(1) != nil {
		ms.error(decl, "func init must have no arguments and no return values")
	}
}

func (ms *multiscope) addImport(node ast.Node, name string, pkg *types.Package) {
	if name == "_" {
		return
	} else if name == "main" {
		ms.error(node, "cannot import package \"main\"")
		return
	} else if name != "." {
		ms.checkRedefined(name, node)
		obj := NewObject(types.ImportObj, name, node, ms.currfile)
		if pkg != nil {
			obj.SetValue(pkg)
		}
		ms.getFile().Insert(obj)
		return
	}
	// dot import
	if pkg == nil {
		return
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
		ms.checkRedefined(name, node)
		ms.getFile().Insert((*Object)(obj)) // reuse the same object
	}
}

func (ms *multiscope) checkRedefined(name string, node ast.Node) {
	if old, _ := ms.lookup(name); old != nil {
		ms.errorRedefined(name, node, old)
	} else if ms.outer.Lookup(name) != nil {
		ms.warning(node, name+" redeclared in this block")
	}
}

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
	outer *types.Scope                 // existing scope being extended
	scope *types.Scope                 // declared symbols
	files map[*token.File]*types.Scope // per-file symbols: imports and dot imports
	curr  *token.File                  // current file
	errors
}

// does NOT clear accumulated errors
func (ms *multiscope) Init(fileset *token.FileSet, outer *types.Scope) {
	ms.outer = outer
	ms.scope = nil
	ms.files = nil
	ms.curr = nil
	ms.errors.fileset = fileset
}

func (ms *multiscope) lookup(name string) *types.Object {
	obj := ms.files[ms.curr].Lookup(name)
	if obj == nil {
		obj = ms.scope.Lookup(name)
	}
	return obj
}

func (ms *multiscope) getFile() *types.Scope {
	file := ms.files[ms.curr]
	if file == nil {
		file = ms.getFileSlow()
	}
	return file
}

func (ms *multiscope) getFileSlow() *types.Scope {
	scope := ms.getScope()
	file := types.NewScope(scope)
	files := ms.files
	if files == nil {
		files = make(map[*token.File]*types.Scope)
		ms.files = files
	}
	ms.files[ms.curr] = file
	return file
}

func (ms *multiscope) getScope() *types.Scope {
	scope := ms.scope
	if scope == nil {
		scope = types.NewScope(ms.outer.Parent())
		ms.scope = scope
	}
	return scope
}

func (ms *multiscope) add(node ast.Node, cls types.Class, name string, typ ast.Node, init ast.Node, index int) {
	if name == "_" {
		return
	}
	d := &decl{node: node, typ: typ, init: init, index: index, file: ms.curr}
	ms.checkRedefined(name, node)
	obj := types.NewObject(cls, name, nil)
	obj.SetValue(d)
	ms.getScope().Insert(obj)
}

func (ms *multiscope) addImport(node ast.Node, name string, pkg *types.Package) {
	if name == "_" {
		return
	}
	if name != "." {
		d := &decl{node: node, value: pkg, file: ms.curr}
		ms.addImportDecl(types.ImportObj, name, d)
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
		ms.getFile().Insert(obj) // reuse the same object
	}
}

func (ms *multiscope) addImportDecl(cls types.Class, name string, d *decl) {
	ms.checkRedefined(name, d.node)
	obj := types.NewObject(cls, name, nil)
	obj.SetValue(d)
	ms.getFile().Insert(obj)
}

func (ms *multiscope) checkRedefined(name string, node ast.Node) {
	if old := ms.lookup(name); old != nil {
		ms.errorRedefined(name, node, old)
	} else if ms.outer.Lookup(name) != nil {
		ms.warning(node, name+" redeclared in this block")
	}
}

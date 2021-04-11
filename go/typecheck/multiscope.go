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
	outer *types.Scope              // existing scope being extended
	syms  SymbolMap                 // declared symbols
	files map[*token.File]SymbolMap // per-file symbols: imports and dot imports
	curr  *token.File               // current file
	errors
}

// does NOT clear accumulated errors
func (ms *multiscope) Init(fileset *token.FileSet, outer *types.Scope) {
	ms.outer = outer
	ms.syms = nil
	ms.files = nil
	ms.curr = nil
	ms.errors.fileset = fileset
}

func (ms *multiscope) lookup(name string) *Symbol {
	sym := ms.files[ms.curr][name]
	if sym == nil {
		sym = ms.syms[name]
	}
	return sym
}

func (ms *multiscope) getFile() SymbolMap {
	file := ms.files[ms.curr]
	if file == nil {
		file = ms.getFileSlow()
	}
	return file
}

func (ms *multiscope) getFileSlow() SymbolMap {
	file := make(SymbolMap)
	files := ms.files
	if files == nil {
		files = make(map[*token.File]SymbolMap)
		ms.files = files
	}
	ms.files[ms.curr] = file
	return file
}

func (ms *multiscope) getSyms() SymbolMap {
	syms := ms.syms
	if syms == nil {
		syms = make(SymbolMap)
		ms.syms = syms
	}
	return syms
}

func (ms *multiscope) add(node ast.Node, cls types.Class, name string, typ ast.Node, init ast.Node, index int) {
	if name == "_" {
		return
	}
	ms.checkRedefined(name, node)
	sym := NewSymbol(cls, name, node, ms.curr)
	sym.typ = typ
	sym.init = init
	sym.index = index
	ms.getSyms().Insert(sym)
}

func (ms *multiscope) addImport(node ast.Node, name string, pkg *types.Package) {
	if name == "_" {
		return
	}
	if name != "." {
		ms.checkRedefined(name, node)
		sym := NewSymbol(types.ImportObj, name, node, ms.curr)
		sym.obj.SetValue(pkg)
		ms.getFile().Insert(sym)
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
		ms.getFile().InsertObj(obj) // reuse the same object
	}
}

func (ms *multiscope) checkRedefined(name string, node ast.Node) {
	if old := ms.lookup(name); old != nil {
		ms.errorRedefined(name, node, old)
	} else if ms.outer.Lookup(name) != nil {
		ms.warning(node, name+" redeclared in this block")
	}
}

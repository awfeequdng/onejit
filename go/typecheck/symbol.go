/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * symbol.go
 *
 *  Created on: Apr 11, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"fmt"

	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/sort"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

type (
	Symbol struct {
		obj   *types.Object
		node  ast.Node    // node containing the declaration
		typ   ast.Node    // type, may be nil
		init  ast.Node    // initializer expression, may be nil
		index int         // if != NoIndex, must use index-th value from multi-valued init
		file  *token.File // file where symbol is declared. needed to retrieve per-file imports
	}

	SymbolMap map[string]*Symbol

	SymbolSet map[*Symbol]struct{}

	SymbolGraph map[*Symbol]SymbolSet
)

const NoIndex int = -1

func NewSymbol(cls types.Class, name string, node ast.Node, file *token.File) *Symbol {
	return &Symbol{
		obj:  types.NewObject(cls, name, nil),
		node: node,
		file: file,
	}
}

func (sym *Symbol) Object() *types.Object {
	return sym.obj
}

func (sym *Symbol) Class() types.Class {
	return sym.obj.Class()
}

func (sym *Symbol) Name() string {
	return sym.obj.Name()
}

func (sym *Symbol) String() string {
	return sym.obj.String()
}

func (m SymbolMap) Names() []string {
	names := make([]string, len(m))
	i := 0
	for name := range m {
		names[i] = name
		i++
	}
	sort.Strings(names)
	return names
}

func (m SymbolMap) Insert(sym *Symbol) {
	m[sym.Name()] = sym
}

func (m SymbolMap) InsertObj(obj *types.Object) {
	m[obj.Name()] = &Symbol{obj: obj}
}

func (set SymbolSet) Format(f fmt.State, verb rune) {
	fmt.Fprint(f, "[")
	separator := ""
	for sym := range set {
		fmt.Fprint(f, separator)
		separator = ", "
		fmt.Fprint(f, sym.String())
	}
	fmt.Fprint(f, "]")
}

func (g SymbolGraph) Link(from *Symbol, to *Symbol) {
	m := g[from]
	if m == nil {
		m = make(SymbolSet)
		g[from] = m
	}
	m[to] = struct{}{}
}

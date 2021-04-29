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
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/sort"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

type (
	Object types.Object

	// stored in Object.Decl()
	Decl struct {
		node ast.Node // node containing the declaration
		typ  ast.Node // type, may be nil
		init ast.Node // initializer expression, may be nil
		// ConstObj: value of iota
		// VarObj: if != NoIndex, must use index-th value from multi-valued init
		index int
		file  *token.File // file where symbol is declared. needed to retrieve per-file imports

		t types.Type // resolved type, may be nil
	}

	ObjectMap map[string]*Object

	ObjectSet map[*Object]struct{}

	ObjectGraph map[*Object]ObjectSet
)

const NoIndex int = -1

var exists struct{}

func NewObject(cls types.Class, name string, node ast.Node, file *token.File) *Object {
	obj := types.NewObject(cls, name, nil)
	obj.SetDecl(&Decl{node: node, file: file})
	return (*Object)(obj)
}

func (obj *Object) Object() *types.Object {
	return (*types.Object)(obj)
}

func (obj *Object) Class() types.Class {
	return obj.Object().Class()
}

func (obj *Object) Decl() *Decl {
	decl, _ := obj.Object().Decl().(*Decl)
	return decl
}

func (obj *Object) Name() string {
	return obj.Object().Name()
}

func (obj *Object) String() string {
	return obj.Object().String()
}

func (obj *Object) Type() *types.Complete {
	return obj.Object().Type()
}

func (obj *Object) Value() interface{} {
	return obj.Object().Value()
}

func (obj *Object) SetType(typ *types.Complete) {
	obj.Object().SetType(typ)
}

func (obj *Object) SetValue(val interface{}) {
	obj.Object().SetValue(val)
}

func (m ObjectMap) Names() []string {
	names := make([]string, len(m))
	i := 0
	for name := range m {
		names[i] = name
		i++
	}
	sort.Strings(names)
	return names
}

func (m ObjectMap) Insert(obj *Object) {
	m[obj.Name()] = obj
}

func (m ObjectMap) Delete(obj *Object) {
	delete(m, obj.Name())
}

func (m ObjectMap) DeleteSet(set ObjectSet) {
	for obj := range set {
		m.Delete(obj)
	}
}

/*
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
*/

func (g ObjectGraph) Link(from *Object, to *Object) {
	m := g[from]
	if m == nil {
		m = make(ObjectSet)
		g[from] = m
	}
	m[to] = struct{}{}
}

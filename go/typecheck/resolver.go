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
	"github.com/cosmos72/onejit/go/constant"
	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

// resolves symbols i.e. for each declared identifier finds its declaration
// also computes the dependencies among declared identifiers
type Resolver struct {
	multiscope
	objs        map[ast.Node]*Object
	types       map[ast.Node]types.Type     // type of each expression
	values      map[ast.Node]constant.Value // value of each constant
	depfwd      ObjectGraph
	depinv      ObjectGraph
	currpkg     *types.Package
	sources     []ast.Node
	globalsOnly bool
}

// does NOT clear accumulated errors
func (r *Resolver) Init(c *Collector, currpkg *types.Package) {
	r.multiscope = c.multiscope
	r.objs = nil
	r.types = nil
	r.values = nil
	r.depfwd = nil
	r.depinv = nil
	r.currpkg = currpkg
	r.sources = c.sources
	r.globalsOnly = false
}

// resolve global symbols collected by Collector
// and create their *types.Complete
func (r *Resolver) Globals() {
	r.globalsOnly = true
	for _, node := range r.sources {
		r.global(node)
	}
	if len(r.errors.errs) == 0 {
		r.declareGlobals()
	}
}

func (r *Resolver) global(node ast.Node) {
	if node == nil {
		return
	}
	var list ast.Node
	switch op := node.Op(); op {
	case token.DIR:
		r.fileset = node.(*ast.Dir).FileSet
		r.currfile = nil
		list = node
	case token.FILE:
		r.currfile = node.(*ast.File).File
		list = node
	case token.IMPORTS, token.DECLS:
		list = node
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
	if list != nil {
		for i, n := 0, list.Len(); i < n; i++ {
			r.global(list.At(i))
		}
	}
}

func (r *Resolver) funcDecl(decl ast.Node) {
	name := decl.At(1).(*ast.Atom)
	if decl.At(0) /*recv*/ != nil || name.Lit == "init" {
		// TODO methods
		return
	}
	obj := r.ident(nil, name)
	r.node([]*Object{obj}, decl.At(2)) // type

	if r.globalsOnly {
		return
	}
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
	obj := r.ident(nil, spec.At(0))
	if obj == nil {
		return
	}
	r.node([]*Object{obj}, spec.At(1)) // type
}

func (r *Resolver) valueSpec(spec ast.Node) {
	names := spec.At(0)
	n := names.Len()

	idents := make([]*Object, n)
	for i := 0; i < n; i++ {
		obj := r.ident(nil, names.At(i))
		if obj == nil {
			return
		}
		idents[i] = obj
	}

	r.node(idents, spec.At(1)) // type

	// TODO inject iota for CONST declaration

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

func (r *Resolver) node(curr []*Object, node ast.Node) {
	if node == nil {
		return
	}
	// TODO check r.resolved[node]
	switch node.Op() {
	case token.IDENT:
		r.ident(curr, node.(*ast.Atom))
	case token.PERIOD:
		r.qualified(curr, node)
	case token.NAMES:
		// we are *declaring* new names: fields, vars...
	default:
		n := node.Len()
		for i := 0; i < n; i++ {
			r.node(curr, node.At(i))
		}
	}
}

// resolve an identifier
func (r *Resolver) ident(curr []*Object, node ast.Node) *Object {
	obj, imported := r.resolve(node.(*ast.Atom))
	if !imported {
		r.deps(curr, obj)
	}
	return obj
}

func (r *Resolver) resolve(ident *ast.Atom) (obj *Object, imported bool) {
	name := ident.Lit
	obj, imported = r.lookupParent(name)
	if obj == nil {
		r.error(ident, "undefined: "+name)
	} else {
		if r.objs == nil {
			r.objs = make(map[ast.Node]*Object)
		}
		r.objs[ident] = obj
	}
	return obj, imported
}

// resolve a qualified identifier
func (r *Resolver) qualified(curr []*Object, node ast.Node) *Object {
	left, right := node.At(0), node.At(1)
	if right.Op() != token.IDENT {
		r.error(node, "expecting qualified identifier: "+node.String())
		return nil
	}
	if left.Op() != token.IDENT {
		// TODO field or method (expr).b
		r.node(curr, left)
		return nil
	}
	obj := r.ident(curr, left)
	if obj == nil {
		// error was already reported by r.ident()
		return nil
	}
	switch obj.Class() {
	case types.ImportObj:
		return r.qualifiedImport(node, right, obj)
	case types.VarObj, types.TypeObj:
		// TODO field or method a.b
		return nil
	default:
		r.error(node, left.String()+" has no fields or methods: "+node.String())
		return nil
	}
}

func (r *Resolver) qualifiedImport(node ast.Node, right ast.Node, obj *Object) *Object {
	val := obj.Value()
	pkg, _ := val.(*types.Package)
	if pkg == nil {
		if val == nil {
			r.error(node, "package '"+obj.Name()+"' not loaded, cannot access "+node.String())
		} else {
			r.error(node, "internal error, expecting Value() of "+obj.Name()+" to be *types.Package")
		}
		return nil
	}
	pobj := (*Object)(pkg.Scope().Lookup(right.(*ast.Atom).Lit))
	if pobj == nil {
		r.error(node, "undefined: "+node.String())
		return nil
	}
	if r.objs == nil {
		r.objs = make(map[ast.Node]*Object)
	}
	r.objs[node] = pobj
	r.objs[right] = pobj
	return obj
}

// add dependencies fromlist -> to
func (r *Resolver) deps(fromlist []*Object, to *Object) {
	if len(fromlist) == 0 || to == nil {
		return
	}
	// do not store self-recursion dependency: declareObj* methods can handle them
	if len(fromlist) == 1 && fromlist[0] == to {
		return
	}
	if r.depfwd == nil {
		r.depfwd = make(ObjectGraph)
	}
	if r.depinv == nil {
		r.depinv = make(ObjectGraph)
	}
	for _, from := range fromlist {
		r.depfwd.Link(from, to)
		r.depinv.Link(to, from)
	}
}

// create *types.Complete for all global declarations passed to Globals()
func (r *Resolver) declareGlobals() {
	m := dup(r.named)
	for len(m) != 0 {
		obj := r.pickObjectWithoutDeps(m)
		if obj != nil {
			r.declareObj(obj)
			r.removeDeps(obj)
			m.Delete(obj)
			continue
		}

		// FIXME: also allow generic types
		set := r.findObjCycle(m, isTypeObj)
		if set == nil {
			r.error(nil, "invalid recursive declaration")
			break
		}
		r.declareObjTypeCycle(set)
		r.removeDepsSet(set)
		m.DeleteSet(set)
	}
}

func dup(m ObjectMap) ObjectMap {
	ret := make(ObjectMap, len(m))
	for name, obj := range m {
		ret[name] = obj
	}
	return ret
}

func isTypeObj(obj *Object) bool {
	return obj.Class() == types.TypeObj
}

func (r *Resolver) pickObjectWithoutDeps(m ObjectMap) *Object {
	for _, obj := range m {
		if len(r.depfwd[obj]) == 0 {
			return obj
		}
	}
	return nil
}

func (r *Resolver) removeDeps(obj *Object) {
	for fwd := range r.depinv[obj] {
		dep := r.depfwd[fwd]
		delete(dep, obj)
		if len(dep) == 0 {
			delete(r.depfwd, fwd)
		}
	}
	delete(r.depinv, obj)
}

func (r *Resolver) removeDepsSet(set ObjectSet) {
	for obj := range set {
		r.removeDeps(obj)
	}
}

// create *types.Complete for specified Object
func (r *Resolver) declareObj(obj *Object) {
	if obj.Type() != nil {
		return
	}
	switch obj.Class() {
	case types.ConstObj:
		r.declareObjConst(obj)
	case types.FuncObj:
		r.declareObjFunc(obj)
	case types.TypeObj:
		r.declareObjType(obj)
	case types.VarObj:
		r.declareObjVar(obj)
	case types.GenericFuncObj:
		r.declareObjGenericFunc(obj)
	case types.GenericTypeObj:
		r.declareObjGenericType(obj)
	default:
		r.error(obj.Decl().node, "cannot declare a "+obj.Class().String()+" object")
	}
}

func (r *Resolver) declareObjGenericFunc(obj *Object) {
	// TODO
}

func (r *Resolver) declareObjGenericType(obj *Object) {
	// TODO
}

func (r *Resolver) recoverFromPanic(node *ast.Node) {
	r.universe.Lookup("iota").SetValue(nil)
	switch fail := recover().(type) {
	case nil:
	case *token.Error:
		r.errors.errs = append(r.errors.errs, fail)
	case string:
		r.error(*node, fail)
	default:
		panic(fail)
	}
}

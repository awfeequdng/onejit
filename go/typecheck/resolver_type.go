/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * resolver_type.go
 *
 *  Created on: Apr 10, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

func (r *Resolver) declareObjFunc(obj *Object) {
	if obj.Type() != nil {
		return
	}
	decl := obj.Decl()
	if decl == nil {
		r.error(nil, "missing declaration for "+obj.Name())
		return
	}
	defer r.recoverFromPanic(&decl.node)

	t := r.makeType(decl.typ)
	obj.SetType(completeType(t))
}

func (r *Resolver) declareObjType(obj *Object) {
	if obj.Type() != nil {
		return
	}
	decl := obj.Decl()
	if decl == nil {
		r.error(nil, "missing declaration for "+obj.Name())
		return
	}
	defer r.recoverFromPanic(&decl.node)

	if decl.t == nil {
		var named *types.Named
		if decl.init == nil {
			// new named type (not an alias) => allow recursive type
			named = types.NewNamed(obj.Name(), r.currpkg.PkgPath())
			decl.t = named
		}
		underlying := r.makeType(decl.typ)
		if named != nil {
			named.SetUnderlying(underlying)
		} else {
			decl.t = underlying
		}
	}
	obj.SetType(completeType(decl.t))
}

func completeType(t types.Type) *types.Complete {
	return types.CompleteTypes(t)[0]
}

func (r *Resolver) declareObjTypeCycle(set ObjectSet) {
	// FIXME: also support generic types

	objs := make([]*Object, 0, len(set))
	for obj := range set {
		if obj.Type() != nil {
			continue
		}
		objs = append(objs, obj)
		decl := obj.Decl()
		if decl.t == nil && decl.init == nil {
			// new named type (not an alias) forward declaration
			decl.t = types.NewNamed(obj.Name(), r.currpkg.PkgPath())
		}
	}
	var node ast.Node
	defer r.recoverFromPanic(&node)

	ts := make([]types.Type, len(objs))
	for i, obj := range objs {
		decl := obj.Decl()
		t := decl.t
		node = decl.node

		if named, _ := t.(*types.Named); named != nil {
			// declare underlying type
			underlying := r.makeType(decl.typ)
			named.SetUnderlying(underlying)
		} else if t == nil {
			// declare type
			t = r.makeType(decl.typ)
			decl.t = t
		}
		ts[i] = t
	}
	for i, complete := range types.CompleteTypes(ts...) {
		objs[i].SetType(complete)
	}
}

func (r *Resolver) makeType(node ast.Node) (t types.Type) {
	switch node.Op() {
	case token.MUL:
		elem := r.makeType(node.At(0))
		t = types.NewPointer(elem)
	case token.ARRAY:
		elem := r.makeType(node.At(1))
		if length := node.At(0); length != nil {
			t = r.makeTypeArray(length, elem)
		} else {
			t = types.NewSlice(elem)
		}
	case token.CHAN:
		chandir := makeChanDir(node.At(0))
		elem := r.makeType(node.At(1))
		t = types.NewChan(chandir, elem)
	case token.FUNC:
		t = r.makeTypeFunc(node)
	case token.MAP:
		key := r.makeType(node.At(0))
		elem := r.makeType(node.At(1))
		t = types.NewMap(key, elem)
	case token.INTERFACE:
		t = r.makeTypeInterface(node)
	case token.STRUCT:
		t = r.makeTypeStruct(node)
	default:
		obj := r.objs[node]
		if obj != nil {
			decl := obj.Decl()
			if obj.Class() != types.TypeObj {
				r.error(node, "expected type, found "+node.String())
				break
			} else if decl != nil && decl.t != nil {
				t = decl.t
				break
			} else if complete := obj.Type(); complete != nil {
				t = complete.Type()
				break
			}
		}
		r.error(node, "invalid type declaration: "+node.String())
	}
	if t == nil {
		// FIXME: remove this hack when makeType() is finished
		panic(r.newError(node, "failed to create type for "+node.String()))
	}
	return t
}

func makeChanDir(dir ast.Node) types.ChanDir {
	chandir := types.BothDir
	if dir != nil {
		switch dir.Op() {
		case token.RECV_DIR:
			chandir = types.RecvDir
		case token.SEND_DIR:
			chandir = types.SendDir
		}
	}
	return chandir
}

func (r *Resolver) makeTypeArray(length ast.Node, elem types.Type) types.Type {
	vlen := r.constant(length).ToKind(types.Int)
	n, ok := vlen.Uint64()
	if !ok {
		r.error(length, "constant "+vlen.String()+" overflows int")
		return nil
	}
	return types.NewArray(elem, n)
}

func (r *Resolver) makeTypeFunc(node ast.Node) types.Type {
	params, variadic := r.makeFields(node.At(0), true)
	results, _ := r.makeFields(node.At(1), false)
	in := fieldsTypes(params)
	out := fieldsTypes(results)
	return types.NewFunc(in, out, variadic)
}

func (r *Resolver) makeTypeInterface(node ast.Node) types.Type {
	var embedded []types.Type
	var methods []types.Method
	for i, n := 0, node.Len(); i < n; i++ {
		field := node.At(i).(*ast.Field)
		names := field.Names
		if names == nil {
			embedded = append(embedded, r.makeType(field.Type))
		} else if names.Op() == token.NAMES {
			methods = r.appendMethods(methods, names, field.Type)
		} else if names.Op() == token.TYPES {
			// interface { type /*typelist*/ }
			r.error(node, "interface containing typelist not yet implemented")
		} else {
			r.error(node, "unexpected interface body: "+field.String())
		}
	}
	return types.NewInterface(embedded, methods...)
}

func (r *Resolver) makeTypeStruct(node ast.Node) types.Type {
	fields, _ := r.makeFields(node, false)
	return types.NewStruct(fields...)
}

func (r *Resolver) makeFields(list ast.Node, allowEllipsis bool) (fields []types.Field, variadic bool) {
	if list == nil || list.Len() == 0 {
		return nil, false
	}
	n := list.Len()
	variadic = allowEllipsis && list.At(n-1).Op() == token.ELLIPSIS
	fields = make([]types.Field, 0, n)
	for i := 0; i < n; i++ {
		fields = r.appendFields(fields, list.At(i), variadic && i == n-1)
	}
	return fields, variadic
}

func (r *Resolver) appendFields(appendTo []types.Field, node ast.Node, allowEllipsis bool) []types.Field {
	field := node.(*ast.Field)
	typ := field.Type
	if allowEllipsis && typ.Op() == token.ELLIPSIS {
		typ = typ.At(0)
	}
	t := r.makeType(typ)
	tag := fieldTag(field)
	names := field.Names
	if names == nil || names.Len() == 0 {
		return append(appendTo, types.Field{Type: t, Tag: tag, Embedded: true})
	}
	n := names.Len()
	for i := 0; i < n; i++ {
		appendTo = append(appendTo, makeField(names.Nodes[i], r.currpkg, t, tag))
	}
	return appendTo
}

func makeField(name ast.Node, pkg *types.Package, t types.Type, tag string) types.Field {
	namestr := name.(*ast.Atom).Lit
	pkgpath := ""
	if !token.IsExported(namestr) {
		pkgpath = pkg.PkgPath()
	}
	return types.Field{
		Type: t, Name: namestr, PkgPath: pkgpath, Tag: tag, Embedded: len(namestr) == 0,
	}
}

func fieldsTypes(fields []types.Field) []types.Type {
	n := len(fields)
	if n == 0 {
		return nil
	}
	ts := make([]types.Type, n)
	for i := 0; i < n; i++ {
		ts[i] = fields[i].Type
	}
	return ts
}

func fieldTag(field *ast.Field) string {
	if field == nil || field.Tag == nil {
		return ""
	}
	return field.Tag.Lit
}

func (r *Resolver) appendMethods(appendTo []types.Method, names ast.Node, typ ast.Node) []types.Method {
	path := r.currpkg.PkgPath()
	t := r.makeType(typ)
	for i, n := 0, names.Len(); i < n; i++ {
		name := names.At(i).(*ast.Atom).Lit
		pkgpath := ""
		if !token.IsExported(name) {
			pkgpath = path
		}
		appendTo = append(appendTo, types.Method{
			Type: t, Name: name, PkgPath: pkgpath,
		})
	}
	return appendTo
}

// find a loop of mutually dependent objects among specified objs,
// where each object in the loop also satisfies filter(object).
//
// return nil if no loop is found, or if every loop also contains
// objects that do not satisfy filter(object)
func (r *Resolver) findObjCycle(objs ObjectMap, filter func(*Object) bool) ObjectSet {
	for _, obj := range objs {
		set := r.getTransitiveDeps(obj, filter)
		if set != nil {
			return set
		}
	}
	return nil
}

// compute the set of obj's transitive dependencies and return it.
// filter() called on obj and all its transitive dependencies must return true,
// otherwise this method will exit immediately and return nil
func (r *Resolver) getTransitiveDeps(obj *Object, filter func(*Object) bool) ObjectSet {
	if !filter(obj) {
		return nil
	}
	queue := ObjectSet{}
	set := ObjectSet{}
	for obj != nil {
		set[obj] = exists
		for dep := range r.depfwd[obj] {
			if !filter(dep) {
				return nil
			} else if _, ok := set[dep]; !ok {
				queue[dep] = exists
			}
		}
		obj = pick(queue)
	}
	return set
}

// remove one object from set and return it
func pick(set ObjectSet) *Object {
	for obj := range set {
		delete(set, obj)
		return obj
	}
	return nil
}

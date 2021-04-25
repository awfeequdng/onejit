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
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

// create types.Type for specified Object
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

func (r *Resolver) declareObjConst(obj *Object) {
	// TODO
}

func (r *Resolver) declareObjFunc(obj *Object) {
	// TODO
}

func (r *Resolver) declareObjType(obj *Object) {
	if obj.Type() != nil {
		return
	}
	defer func() {
		// FIXME: remove this hack when makeType() is finished
		recover()
	}()
	decl := obj.Decl()
	if decl.t == nil {
		var named *types.Named
		if decl.init == nil {
			// new named type (not an alias) => allow recursive type
			named = types.NewNamed(obj.Name(), r.currpkg.PkgPath())
			decl.t = named
		}
		underlying := r.makeType(decl.typ)
		if named != nil {
			named.SetUnderlying(underlying.Underlying())
		} else {
			decl.t = underlying
		}
	}
	obj.SetType(types.CompleteTypes(decl.t)[0])
}

func (r *Resolver) declareObjVar(obj *Object) {
	// TODO
}

func (r *Resolver) declareObjGenericFunc(obj *Object) {
	// TODO
}

func (r *Resolver) declareObjGenericType(obj *Object) {
	// TODO
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
		obj := r.resolved[node]
		if obj != nil {
			decl := obj.Decl()
			if decl != nil && decl.t != nil {
				t = decl.t
				break
			} else if complete := obj.Type(); complete != nil {
				t = complete.Type()
				break
			}
		}
		r.error(node, "unsupported type declaration: "+node.String())
	}
	if t == nil {
		// FIXME: remove this hack when makeType() is finished
		panic("Resolver.makeType unimplemented for " + node.String())
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
	// TODO
	return nil
}

func (r *Resolver) makeTypeFunc(node ast.Node) types.Type {
	// TODO
	return nil
}

func (r *Resolver) makeTypeInterface(node ast.Node) types.Type {
	// TODO
	return nil
}

func (r *Resolver) makeTypeStruct(node ast.Node) types.Type {
	// TODO
	return nil
}

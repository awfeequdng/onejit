/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * resolver_const.go
 *
 *  Created on: Apr 10, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/constant"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

func (r *Resolver) declareObjVar(obj *Object) {
	if obj.Type() != nil {
		return
	}
	decl := obj.Decl()
	if decl == nil {
		r.error(nil, "missing declaration for "+obj.Name())
		return
	} else if decl.init == nil && decl.typ == nil {
		return
	}
	defer r.recoverFromPanic(&decl.node)

	var t types.Type
	var v constant.Value

	if decl.init != nil {
		if decl.index == NoIndex {
			// one initializer per variable
			t, v = r.expr(decl.init)
		} else {
			// TODO
		}
	}
	if decl.typ != nil {
		// TODO if t != nil, convert to it
		t = r.makeType(decl.typ)
	}
	if v.IsValid() {
		// TODO this is the INITIAL value of the variable
		// obj.SetValue(v)

		// TODO untyped constants have v.Type().Type() == nil
		// if t == nil, use default type of untyped constant
	}
	if t != nil {
		obj.SetType(completeType(t))
	}
}

func (r *Resolver) declareObjConst(obj *Object) {
	if obj.Value() != nil && obj.Type() != nil {
		return
	}
	decl := obj.Decl()
	if decl == nil || decl.node == nil {
		r.error(nil, "missing declaration for "+obj.Name())
		return
	} else if decl.init == nil && decl.typ == nil {
		r.error(decl.node, "missing type or value in const declaration: "+decl.node.String())
		return
	}
	r.universe.Lookup("iota").SetValue(constant.MakeKind(token.UntypedInt, decl.index))
	// also removes iota value
	defer r.recoverFromPanic(&decl.node)

	var v constant.Value
	var t *types.Complete
	if decl.init != nil {
		v = r.constant(decl.init)
		t = v.Type()
	}
	if decl.typ != nil {
		t = completeType(r.makeType(decl.typ))
		if decl.init == nil {
			v = constant.MakeZero(t)
		} else {
			v = v.To(t)
		}
	}
	obj.SetType(t)
	obj.SetValue(v)
}

// resolve the value of a constant.
func (r *Resolver) constant(node ast.Node) constant.Value {
	if obj := r.objs[node]; obj != nil && obj.Class() == types.ConstObj {
		if v, ok := obj.Value().(constant.Value); ok {
			return v
		}
	}
	var v constant.Value // the zero value is Invalid
	if op := node.Op(); op.IsLiteral() && op != token.IDENT {
		atom := node.(*ast.Atom)
		v = constant.MakeFromLiteral(atom.Lit, op)
	} else if op.IsOperator() {
		if node.Len() == 1 {
			v = r.constant(node.At(0))
			v = constant.UnaryOp(op, v)
		} else if node.Len() == 2 {
			xv := r.constant(node.At(0))
			yv := r.constant(node.At(1))
			v = constant.BinaryOp(xv, op, yv)
		}
	} else if op == token.CALL {
		// TODO
		r.error(node, "unimplemented: type conversion on constant")
	}
	if v.Kind() == constant.Invalid {
		r.error(node, "const initializer "+node.String()+" is not a constant")
	} else {
		r.setTypeValue(node, v.Type().Type(), v)
	}
	return v
}

// resolve the type of an expression. if the expression is a constant, also returns its value
func (r *Resolver) expr(node ast.Node) (t types.Type, v constant.Value) {
	if obj := r.objs[node]; obj != nil {
		t = obj.Type().Type()
		switch obj.Class() {
		case types.ConstObj:
			if v_, ok := obj.Value().(constant.Value); ok {
				v = v_
			}
			return t, v
		case types.VarObj, types.FuncObj:
			return t, v
		default:
			r.error(node, "not an expression: "+node.String())
			return nil, v
		}
	}
	switch op := node.Op(); op {
	case token.CALL:
		t, v = r.call(node)
	case token.COMPOSITE_LIT:
		t, v = r.compositeLit(node)
	default:
		switch n := node.Len(); n {
		case 0:
			t, v = r.atom(node)
		case 1:
			t, v = r.unary(node)
		case 2:
			t, v = r.binary(node)
		default:
			// TODO what here?
			r.error(node, "unsupported n-arg expression: "+node.String())
			return t, v
		}
	}
	r.setTypeValue(node, t, v)
	return t, v
}

// resolve the type of a 0-arg expression.
// if the expression is a constant, also returns its value
func (r *Resolver) atom(node ast.Node) (t types.Type, v constant.Value) {
	if node.Op().IsLiteral() {
		v = r.constant(node)
		t = v.Type().Type()
	} else {
		// TODO what here?
		r.error(node, "unsupported 0-arg expression: "+node.String())
	}
	return t, v
}

// resolve the type of an unary expression. if the expression is a constant, also returns its value
func (r *Resolver) unary(node ast.Node) (t types.Type, v constant.Value) {
	// TODO
	return t, v
}

// resolve the type of a binary expression. if the expression is a constant, also returns its value
func (r *Resolver) binary(node ast.Node) (t types.Type, v constant.Value) {
	// TODO
	return t, v
}

// resolve the type of a call. if the call is a constant, also returns its value
func (r *Resolver) call(node ast.Node) (t types.Type, v constant.Value) {
	// TODO
	return t, v
}

// resolve the type of a composite literal
func (r *Resolver) compositeLit(node ast.Node) (t types.Type, v constant.Value) {
	// TODO
	return t, v
}

func (r *Resolver) setTypeValue(node ast.Node, t types.Type, v constant.Value) {
	if v.IsValid() {
		if r.values == nil {
			r.values = make(map[ast.Node]constant.Value)
		}
		r.values[node] = v
	}
	if t != nil {
		if r.types == nil {
			r.types = make(map[ast.Node]types.Type)
		}
		r.types[node] = t
	}
}

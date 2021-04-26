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

func (r *Resolver) declareObjConst(obj *Object) {
	// TODO
}

func (r *Resolver) makeConst(node ast.Node) constant.Value {
	var v constant.Value // the zero value is Invalid
	if obj := r.resolved[node]; obj != nil && obj.Class() == types.ConstObj {
		if objv, ok := obj.Value().(constant.Value); ok {
			return objv
		}
	}
	if op := node.Op(); op.IsLiteral() {
		atom := node.(*ast.Atom)
		v = constant.MakeFromLiteral(atom.Lit, op)
	} else if op.IsOperator() {
		if node.Len() == 1 {
			v = r.makeConst(node.At(0))
			v = constant.UnaryOp(op, v)
		} else if node.Len() == 2 {
			xv := r.makeConst(node.At(0))
			yv := r.makeConst(node.At(1))
			v = constant.BinaryOp(xv, op, yv)
		}
	} else if op == token.CALL {
		// TODO
		r.error(node, "unimplemented: type conversion on constant")
	}
	if v.Kind() == constant.Invalid {
		r.error(node, "const initializer "+node.String()+" is not a constant")
	}
	return v
}

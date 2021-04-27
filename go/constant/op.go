/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * op.go
 *
 *  Created on: Apr 17, 2021
 *      Author: Massimiliano Ghilardi
 */

package constant

import (
	"go/constant"
	gotoken "go/token"

	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

// equivalent to go/constant.UnaryOp()
func UnaryOp(op token.Token, v Value) Value {
	if !v.IsValid() {
		return v
	}
	c := constant.UnaryOp(gotoken.Token(op), v.cval, uint(v.Kind().Size()*8))
	return validate(c, v.typ)
}

// replaces go/constant.BinaryOp() go/constant.Compare() and go/constant.Shift()
func BinaryOp(xv Value, op token.Token, yv Value) Value {
	if !xv.IsValid() || !yv.IsValid() {
		return invalid
	}
	if op == token.SHL || op == token.SHR {
		return shift(xv, op, yv)
	}
	t := combine(xv.typ, yv.typ)
	if t == nil {
		return Value{&value{cunknown, nil, errMismatchedKinds(xv, op, yv)}}
	}
	var c constant.Value
	switch op {
	case token.EQL, token.LSS, token.GTR, token.NEQ, token.LEQ, token.GEQ:
		flag := constant.Compare(xv.cval, gotoken.Token(op), yv.cval)
		c = constant.MakeBool(flag)
		t = types.BasicType(UntypedBool)
	default:
		c = constant.BinaryOp(xv.cval, gotoken.Token(op), yv.cval)
	}
	return validate(c, t)
}

// equivalent to go/constant.Shift()
func shift(xv Value, op token.Token, yv Value) Value {
	yv = yv.To(types.BasicType(Uint32))
	if !yv.IsValid() {
		return yv
	}
	y, _ := yv.Uint64()
	if xkind := xv.Kind(); xkind.IsUntyped() {
		if xkind != UntypedInt && xkind != UntypedRune {
			xv = xv.To(types.BasicType(UntypedInt))
			if !xv.IsValid() {
				return xv
			}
		}
	} else if !xkind.IsInteger() {
		return Value{&value{cunknown, nil,
			ErrorType{"invalid shift: " + xv.String() + " " + op.String() + " " +
				strings.Uint64ToString(y) + " left operand",
				types.BasicType(Int)}}}
	}
	xc := constant.Shift(xv.cval, gotoken.Token(op), uint(y))
	return validate(xc, xv.typ)
}

func combine(xt *types.Complete, yt *types.Complete) *types.Complete {
	if xt == nil || yt == nil {
		return nil
	} else if xt == yt {
		return xt
	}
	x, y := xt.Kind(), yt.Kind()
	// ensure x is >= y
	if x < y {
		x, y = y, x
		xt, yt = yt, xt
	}
	if isTyped(x) && isTyped(y) {
		return nil
	} else if isTyped(x) {
		return xt
	} else if isTyped(y) {
		return yt
	} else if isUntypedNumeric(x) && isUntypedNumeric(y) {
		return xt
	} else {
		return nil
	}
}
func isTyped(k Kind) bool {
	return !k.IsUntyped()
}

func isUntypedNumeric(k Kind) bool {
	return k >= UntypedInt && k <= UntypedComplex
}

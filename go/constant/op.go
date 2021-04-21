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
)

// equivalent to go/constant.UnaryOp()
func UnaryOp(op token.Token, v Value) Value {
	c := constant.UnaryOp(gotoken.Token(op), v.cval, uint(v.kind.Size()*8))
	return validate(c, v.kind)
}

// replaces go/constant.BinaryOp() go/constant.Compare() and go/constant.Shift()
func BinaryOp(xv Value, op token.Token, yv Value) Value {
	if op == token.SHL || op == token.SHR {
		return shift(xv, op, yv)
	}
	kind := combine(xv.kind, yv.kind)
	if kind == Invalid {
		return Value{&value{cunknown, Invalid, errMismatchedKinds(xv, op, yv)}}
	}
	var c constant.Value
	switch op {
	case token.EQL, token.LSS, token.GTR, token.NEQ, token.LEQ, token.GEQ:
		flag := constant.Compare(xv.cval, gotoken.Token(op), yv.cval)
		c = constant.MakeBool(flag)
		kind = UntypedBool
	default:
		c = constant.BinaryOp(xv.cval, gotoken.Token(op), yv.cval)
	}
	return validate(c, kind)
}

// equivalent to go/constant.Shift()
func shift(xv Value, op token.Token, yv Value) Value {
	yv = yv.To(Uint32)
	if !yv.IsValid() {
		return yv
	}
	y, _ := yv.Uint64()
	if xv.kind.IsUntyped() {
		xv = xv.To(UntypedInt)
		if !xv.IsValid() {
			return xv
		}
	} else if !xv.kind.IsInteger() {
		return Value{&value{cunknown, Invalid,
			ErrorKind{"invalid shift: " + xv.String() + " " + op.String() + " " +
				strings.Uint64ToString(y) + " left operand", Int}}}
	}
	xc := constant.Shift(xv.cval, gotoken.Token(op), uint(y))
	return validate(xc, xv.kind)
}

func combine(x Kind, y Kind) Kind {
	// ensure x is >= y
	if x < y {
		x, y = y, x
	}
	if x == y {
		return x
	} else if isTyped(x) && isTyped(y) {
		return Invalid
	} else if isTyped(x) {
		return x
	} else if isTyped(y) {
		return y
	} else if isUntypedNumeric(x) && isUntypedNumeric(y) {
		return x
	} else {
		return Invalid
	}
}
func isTyped(k Kind) bool {
	return !k.IsUntyped()
}

func isUntypedNumeric(k Kind) bool {
	return k >= UntypedInt && k <= UntypedComplex
}

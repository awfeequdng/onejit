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
func UnaryOp(op token.Token, v *Value) (*Value, error) {
	c := constant.UnaryOp(gotoken.Token(op), v.cval, uint(v.kind.Size()*8))
	return validate(c, v.kind)
}

// equivalent to go/constant.BinaryOp()
func BinaryOp(xv *Value, op token.Token, yv *Value) (*Value, error) {
	xkind, ykind := xv.kind, yv.kind
	if xv.kind != yv.kind && !xkind.IsUntyped() && !ykind.IsUntyped() {
		return nil, errMismatchedKinds(xv, op, yv)
	}
	c := constant.BinaryOp(xv.cval, gotoken.Token(op), yv.cval)
	return validate(c, xv.kind)
}

// equivalent to go/constant.Compare()
func Compare(xv *Value, op token.Token, yv *Value) (bool, error) {
	xkind, ykind := xv.kind, yv.kind
	if xv.kind != yv.kind && !xkind.IsUntyped() && !ykind.IsUntyped() {
		return false, errMismatchedKinds(xv, op, yv)
	}
	return constant.Compare(xv.cval, gotoken.Token(op), yv.cval), nil
}

// equivalent to go/constant.Shift()
func Shift(xv *Value, op token.Token, yv *Value) (*Value, error) {
	yv, err := yv.To(Uint32)
	if err != nil {
		return nil, err
	}
	y, _ := yv.Uint64()
	if xv.kind.IsUntyped() {
		xv, err = xv.To(UntypedInt)
		if err != nil {
			return nil, err
		}
	} else if !xv.kind.IsInteger() {
		return nil, ErrorKind{"invalid shift: " + xv.String() + " " + op.String() + " " +
			strings.Uint64ToString(y) + " left operand", Int}
	}
	xc := constant.Shift(xv.cval, gotoken.Token(op), uint(y))
	return validate(xc, xv.kind)
}

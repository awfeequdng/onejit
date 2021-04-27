/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * error.go
 *
 *  Created on: Apr 12, 2021
 *      Author: Massimiliano Ghilardi
 */

package constant

import (
	"go/constant"

	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

type ErrorInvalid struct {
}

type ErrorOverflow struct {
	cval constant.Value
	kind Kind
}

type ErrorType struct {
	str string
	typ *types.Complete
}

func (e ErrorInvalid) Error() string {
	return "constant is Invalid"
}

func (e ErrorOverflow) Error() string {
	return "constant " + e.cval.String() + " overflows " + e.kind.String()
}

func (e ErrorType) Error() string {
	if e.typ == nil {
		return e.str
	}
	return e.str + " is not " + e.typ.String()
}

func (e ErrorType) Type() *types.Complete {
	return e.typ
}

var (
	ErrInvalid = ErrorInvalid{}
)

func errMismatchedKinds(xv Value, op token.Token, yv Value) error {
	xt, yt := xv.Type(), yv.Type()
	return ErrorType{"invalid operation: " + xv.String() + " " + op.String() + " " + yv.String() +
		" (mismatched types " + xt.String() + " and " + yt.String() + " )", nil}
}

func errNotReal(c constant.Value) Value {
	return Value{&value{cunknown, nil, ErrorType{"constant " + c.String() + " not integer, rune or float", nil}}}
}

func errNotNumeric(c constant.Value) Value {
	return Value{&value{cunknown, nil, ErrorType{"constant " + c.String() + " not numeric", nil}}}
}

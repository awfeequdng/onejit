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
)

type ErrorInvalid struct {
}

type ErrorOverflow struct {
	cval constant.Value
	kind Kind
}

type ErrorKind struct {
	str  string
	kind Kind
}

func (e ErrorInvalid) Error() string {
	return "constant is Invalid"
}

func (e ErrorOverflow) Error() string {
	return "constant " + e.cval.String() + " overflows " + e.kind.String()
}

func (e ErrorKind) Error() string {
	if e.kind == Invalid {
		return e.str
	}
	return e.str + " is not " + e.kind.String()
}

func (e ErrorKind) Kind() Kind {
	return e.kind
}

var (
	ErrInvalid = ErrorInvalid{}
)

func errMismatchedKinds(xv Value, op token.Token, yv Value) error {
	xkind, ykind := xv.kind, yv.kind
	return ErrorKind{"invalid operation: " + xv.String() + " " + op.String() + " " + yv.String() +
		" (mismatched kinds " + xkind.String() + " and " + ykind.String() + " )", Invalid}
}

func errNotReal(c constant.Value) Value {
	return Value{&value{cunknown, Invalid, ErrorKind{"constant " + c.String() + " not integer, rune or float", Invalid}}}
}

func errNotNumeric(c constant.Value) Value {
	return Value{&value{cunknown, Invalid, ErrorKind{"constant " + c.String() + " not numeric", Invalid}}}
}

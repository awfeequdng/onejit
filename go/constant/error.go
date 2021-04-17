/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * value.go
 *
 *  Created on: Apr 12, 2021
 *      Author: Massimiliano Ghilardi
 */

package constant

import "go/constant"

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
	return e.str + " is not " + e.kind.String()
}

var (
	ErrInvalid = ErrorInvalid{}
)

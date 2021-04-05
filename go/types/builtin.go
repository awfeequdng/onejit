/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * builtin.go
 *
 *  Created on: Apr 05, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

// create a new type representing a builtin function
func NewBuiltin(nin uint32, nout uint32, variadic bool) *Basic {
	t := &Basic{
		rtype: Complete{
			size:  archSizeBytes,
			flags: flagComplete | flagNotComparable,
			kind:  Invalid,
			str:   "func(...)",
		},
	}
	t.rtype.typ = t
	t.rtype.extra = &extra{
		n1:    nin,
		n2:    nout,
		types: make([]Type, nin+nout),
	}
	return t
}

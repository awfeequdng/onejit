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

// create a new type representing a builtin function.
// the returned *Complete has .Kind() = Invalid and .Type() = nil,
// as it cannot be used as component in new types.
func NewBuiltin(nin uint32, nout uint32, variadic bool) *Complete {
	size := sizeOfPtr()
	return &Complete{
		size:  size,
		align: uint16(size),
		flags: flagComplete | flagNotComparable,
		kind:  Invalid,
		extra: &extra{
			n1:    nin,
			n2:    nout,
			types: make([]Type, nin+nout),
		},
		str: "func(...)",
	}
}

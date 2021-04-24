/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * api.go
 *
 *  Created on: Apr 23, 2021
 *      Author: Massimiliano Ghilardi
 */

// some random code, used for testing package go/typecheck

package testdata

/*
const (
	Foo     = Bar
	Bar int = 1
)
*/

type Pair struct {
	first, second [1]int
}

var Len = len((*Pair)(nil).first)

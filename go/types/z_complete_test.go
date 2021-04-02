/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_test.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"testing"
)

func TestComplete(test *testing.T) {
	/*
	 * runtime equivalent of
	 * type foo bar
	 * type bar *foo
	 */
	foo := NewNamed("foo", "")
	bar := NewNamed("bar", "")
	foo.SetUnderlying(bar)
	bar.SetUnderlying(NewSlice(foo))

	cs := CompleteTypes(foo, bar)
	test.Log(cs)
}

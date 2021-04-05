/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_scope_test.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"fmt"
	"testing"
)

func TestScope(test *testing.T) {
	s := Universe()
	for _, name := range s.Names() {
		obj := s.Lookup(name)
		fmt.Printf("%s => %v %v\n", name, obj.Class, obj.Type)
	}
}

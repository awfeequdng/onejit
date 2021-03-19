/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_number_test.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package scanner

import (
	"testing"

	"github.com/cosmos72/onejit/go/token"
)

func TestDecimal(t *testing.T) {
	v := TestCases{
		TestCase{"0", Item{token.INT, "0"}},
		TestCase{"1", Item{token.INT, "1"}},
		TestCase{"1", Item{token.INT, "1"}},
		TestCase{"3_7", Item{token.INT, "37"}},
		TestCase{"789_123", Item{token.INT, "789123"}},
	}
	v.run(t)
}

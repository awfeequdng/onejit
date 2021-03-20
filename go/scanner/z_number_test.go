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

func TestBinary(t *testing.T) {
	v := TestCases{
		TestCase{"0b", errIntHasNodigits[base2]},
		TestCase{"0b0", Item{token.INT, "0b0"}},
		TestCase{"0b1 ", Item{token.INT, "0b1"}},
		TestCase{"0b2 ", errIntBadDigit[base2]},
		TestCase{"0b_1 ", Item{token.INT, "0b1"}},
		TestCase{"0b_1_ ", errIntBadUnderscore},
		TestCase{"0b__1 ", errIntBadUnderscore},
		TestCase{"0b10101010101 ", Item{token.INT, "0b10101010101"}},
	}
	v.run(t)
}

func TestOctal(t *testing.T) {
	v := TestCases{
		TestCase{"0", Item{token.INT, "0"}},
		TestCase{"01 ", Item{token.INT, "01"}},
		TestCase{"0_1 ", Item{token.INT, "01"}},
		TestCase{"0_1_ ", errIntBadUnderscore},
		TestCase{"0__1 ", errIntBadUnderscore},
		TestCase{"09 ", errIntBadDigit[base8]},
		TestCase{"0123456701234567 ", Item{token.INT, "0123456701234567"}},
		TestCase{"0o", errIntHasNodigits[base8]},
		TestCase{"0o0", Item{token.INT, "0o0"}},
		TestCase{"0o1 ", Item{token.INT, "0o1"}},
		TestCase{"0o_1 ", Item{token.INT, "0o1"}},
		TestCase{"0o_1_ ", errIntBadUnderscore},
		TestCase{"0o__1 ", errIntBadUnderscore},
		TestCase{"0o9 ", errIntBadDigit[base8]},
		TestCase{"0o123456701234567 ", Item{token.INT, "0o123456701234567"}},
		TestCase{"0_b", errSyntaxErrorUnexpectedChar},
		TestCase{"0_o", errSyntaxErrorUnexpectedChar},
		TestCase{"0_x", errSyntaxErrorUnexpectedChar},
	}
	v.run(t)
}

func TestDecimal(t *testing.T) {
	v := TestCases{
		TestCase{"1", Item{token.INT, "1"}},
		TestCase{"12 ", Item{token.INT, "12"}},
		TestCase{
			"123456789012345678901234567890",
			Item{token.INT, "123456789012345678901234567890"},
		},
		TestCase{"1_2 ", Item{token.INT, "12"}},
		TestCase{"1_2_ ", errIntBadUnderscore},
		TestCase{"1__2 ", errIntBadUnderscore},
		TestCase{"1_2_x ", errSyntaxErrorUnexpectedChar},
		TestCase{"123_456 ", Item{token.INT, "123456"}},
	}
	v.run(t)
}

func TestHexadecimal(t *testing.T) {
	v := TestCases{
		TestCase{"0x", errIntHasNodigits[base16]},
		TestCase{"0x0", Item{token.INT, "0x0"}},
		TestCase{"0x1 ", Item{token.INT, "0x1"}},
		TestCase{"0x_1 ", Item{token.INT, "0x1"}},
		TestCase{"0x_1_ ", errIntBadUnderscore},
		TestCase{"0x__1 ", errIntBadUnderscore},
		TestCase{"0x9g ", errSyntaxErrorUnexpectedChar},
		TestCase{"0x123456789abcdef ", Item{token.INT, "0x123456789abcdef"}},
		TestCase{"0_b", errSyntaxErrorUnexpectedChar},
		TestCase{"0_o", errSyntaxErrorUnexpectedChar},
		TestCase{"0_x", errSyntaxErrorUnexpectedChar},
	}
	v.run(t)
}

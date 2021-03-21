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

func TestIntBinary(t *testing.T) {
	v := TestCases{
		TestCase{"0b", Item{token.ILLEGAL, "0b"}, errIntHasNodigits[base2]},
		TestCase{"0b0", Item{token.INT, "0b0"}, nil},
		TestCase{"0b1 ", Item{token.INT, "0b1"}, nil},
		TestCase{"0b2 ", Item{token.ILLEGAL, "0b2"}, errIntBadDigit[base2]},
		TestCase{"0b_1 ", Item{token.INT, "0b_1"}, nil},
		TestCase{"0b_1_ ", Item{token.ILLEGAL, "0b_1_"}, errIntBadUnderscore},
		TestCase{"0b__1 ", Item{token.ILLEGAL, "0b__"}, errIntBadUnderscore},
		TestCase{"0b10101010101 ", Item{token.INT, "0b10101010101"}, nil},
	}
	v.run(t)
}

func TestIntOctal(t *testing.T) {
	v := TestCases{
		TestCase{"0", Item{token.INT, "0"}, nil},
		TestCase{"01 ", Item{token.INT, "01"}, nil},
		TestCase{"0_1 ", Item{token.INT, "0_1"}, nil},
		TestCase{"0_1_ ", Item{token.ILLEGAL, "0_1_"}, errIntBadUnderscore},
		TestCase{"0__1 ", Item{token.ILLEGAL, "0__"}, errIntBadUnderscore},
		TestCase{"09 ", Item{token.ILLEGAL, "09"}, errIntBadDigit[base8]},
		TestCase{"0123_456701234_567 ", Item{token.INT, "0123_456701234_567"}, nil},
		TestCase{"0o", Item{token.ILLEGAL, "0o"}, errIntHasNodigits[base8]},
		TestCase{"0o0", Item{token.INT, "0o0"}, nil},
		TestCase{"0o1 ", Item{token.INT, "0o1"}, nil},
		TestCase{"0o_1 ", Item{token.INT, "0o_1"}, nil},
		TestCase{"0o_1_ ", Item{token.ILLEGAL, "0o_1_"}, errIntBadUnderscore},
		TestCase{"0o__1 ", Item{token.ILLEGAL, "0o__"}, errIntBadUnderscore},
		TestCase{"0o9 ", Item{token.ILLEGAL, "0o9"}, errIntBadDigit[base8]},
		TestCase{"0o1234_56701_234567 ", Item{token.INT, "0o1234_56701_234567"}, nil},
		TestCase{"0_b", Item{token.ILLEGAL, "0_b"}, errSyntaxErrorUnexpectedChar},
		TestCase{"0_o", Item{token.ILLEGAL, "0_o"}, errSyntaxErrorUnexpectedChar},
		TestCase{"0_x", Item{token.ILLEGAL, "0_x"}, errSyntaxErrorUnexpectedChar},
	}
	v.run(t)
}

func TestIntDecimal(t *testing.T) {
	v := TestCases{
		TestCase{"1", Item{token.INT, "1"}, nil},
		TestCase{"12 ", Item{token.INT, "12"}, nil},
		TestCase{
			"1234567890_1234567890123456789_0",
			Item{token.INT, "1234567890_1234567890123456789_0"},
			nil,
		},
		TestCase{"1_2 ", Item{token.INT, "1_2"}, nil},
		TestCase{"1_2_ ", Item{token.ILLEGAL, "1_2_"}, errIntBadUnderscore},
		TestCase{"1__2 ", Item{token.ILLEGAL, "1__"}, errIntBadUnderscore},
		TestCase{"1_2_x ", Item{token.ILLEGAL, "1_2_x"}, errSyntaxErrorUnexpectedChar},
		TestCase{"123_456 ", Item{token.INT, "123_456"}, nil},
	}
	v.run(t)
}

func TestIntHexadecimal(t *testing.T) {
	v := TestCases{
		TestCase{"0x", Item{token.ILLEGAL, "0x"}, errIntHasNodigits[base16]},
		TestCase{"0x0", Item{token.INT, "0x0"}, nil},
		TestCase{"0x1 ", Item{token.INT, "0x1"}, nil},
		TestCase{"0x_1 ", Item{token.INT, "0x_1"}, nil},
		TestCase{"0x_1_ ", Item{token.ILLEGAL, "0x_1_"}, errIntBadUnderscore},
		TestCase{"0x__1 ", Item{token.ILLEGAL, "0x__"}, errIntBadUnderscore},
		TestCase{"0x9g ", Item{token.ILLEGAL, "0x9g"}, errSyntaxErrorUnexpectedChar},
		TestCase{"0x1234_56789abc_def0 ", Item{token.INT, "0x1234_56789abc_def0"}, nil},
	}
	v.run(t)
}

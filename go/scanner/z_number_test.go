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

func TestBinaryInt(t *testing.T) {
	v := SingleTests{
		{"0b", item{token.ILLEGAL, "0b"}, errNumberHasNodigits[base2]},
		{"0b0", item{token.INT, "0b0"}, nil},
		{"0b1 ", item{token.INT, "0b1"}, nil},
		{"0b2 ", item{token.ILLEGAL, "0b2"}, errIntBadDigit[base2]},
		{"0b_1 ", item{token.INT, "0b_1"}, nil},
		{"0b_1_ ", item{token.ILLEGAL, "0b_1_"}, errIntBadUnderscore},
		{"0b__1 ", item{token.ILLEGAL, "0b__"}, errIntBadUnderscore},
		{"0b10101010101 ", item{token.INT, "0b10101010101"}, nil},
	}
	v.run(t)
}

func TestOctalInt(t *testing.T) {
	v := SingleTests{
		// TestCase{"0", item{token.INT, "0"}, nil},
		{"01 ", item{token.INT, "01"}, nil},
		{"0_1 ", item{token.INT, "0_1"}, nil},
		{"0_1_ ", item{token.ILLEGAL, "0_1_"}, errIntBadUnderscore},
		{"0__1 ", item{token.ILLEGAL, "0__"}, errIntBadUnderscore},
		{"09 ", item{token.ILLEGAL, "09"}, errIntBadDigit[base8]},
		{"0123_456701234_567 ", item{token.INT, "0123_456701234_567"}, nil},
		{"0o", item{token.ILLEGAL, "0o"}, errNumberHasNodigits[base8]},
		{"0o0", item{token.INT, "0o0"}, nil},
		{"0o1 ", item{token.INT, "0o1"}, nil},
		{"0o_1 ", item{token.INT, "0o_1"}, nil},
		{"0o_1_ ", item{token.ILLEGAL, "0o_1_"}, errIntBadUnderscore},
		{"0o__1 ", item{token.ILLEGAL, "0o__"}, errIntBadUnderscore},
		{"0o9 ", item{token.ILLEGAL, "0o9"}, errIntBadDigit[base8]},
		{"0o1234_56701_234567 ", item{token.INT, "0o1234_56701_234567"}, nil},
		{"0_b", item{token.ILLEGAL, "0_b"}, errIntBadUnderscore},
		{"0_o", item{token.ILLEGAL, "0_o"}, errIntBadUnderscore},
		{"0_x", item{token.ILLEGAL, "0_x"}, errIntBadUnderscore},
	}
	v.run(t)
}

func TestDecimalInt(t *testing.T) {
	v := SingleTests{
		{"1", item{token.INT, "1"}, nil},
		{"12 ", item{token.INT, "12"}, nil},
		{
			"1234567890_1234567890123456789_0",
			item{token.INT, "1234567890_1234567890123456789_0"},
			nil,
		},
		{"1_2 ", item{token.INT, "1_2"}, nil},
		{"1_2_ ", item{token.ILLEGAL, "1_2_"}, errIntBadUnderscore},
		{"1__2 ", item{token.ILLEGAL, "1__"}, errIntBadUnderscore},
		{"1_2_x ", item{token.ILLEGAL, "1_2_x"}, errIntBadUnderscore},
		{"123_456 ", item{token.INT, "123_456"}, nil},
	}
	v.run(t)
}

func TestHexadecimalInt(t *testing.T) {
	v := SingleTests{
		{"0x", item{token.ILLEGAL, "0x"}, errNumberHasNodigits[base16]},
		{"0x0", item{token.INT, "0x0"}, nil},
		{"0x1 ", item{token.INT, "0x1"}, nil},
		{"0x_1 ", item{token.INT, "0x_1"}, nil},
		{"0x_1_ ", item{token.ILLEGAL, "0x_1_"}, errIntBadUnderscore},
		{"0x__1 ", item{token.ILLEGAL, "0x__"}, errIntBadUnderscore},
		{"0x9g ", item{token.ILLEGAL, "0x9g"}, errSyntaxErrorUnexpectedChar},
		{"0x1234_56789abc_def0 ", item{token.INT, "0x1234_56789abc_def0"}, nil},
	}
	v.run(t)
}

func TestDecimalFloat(t *testing.T) {
	v := SingleTests{
		{"1. ", item{token.FLOAT, "1."}, nil},
		{".0 ", item{token.FLOAT, ".0"}, nil},
		{"7e3 ", item{token.FLOAT, "7e3"}, nil},
		{"1e-09 ", item{token.FLOAT, "1e-09"}, nil},
		{"012_345_6e-7_8 ", item{token.FLOAT, "012_345_6e-7_8"}, nil},
		{".i ", item{token.PERIOD, ""}, nil},
		{".0i ", item{token.IMAG, ".0i"}, nil},
		{"0i ", item{token.IMAG, "0i"}, nil},
		{"0.i ", item{token.IMAG, "0.i"}, nil},
		{"7_8_9e-0_1i ", item{token.IMAG, "7_8_9e-0_1i"}, nil},
	}
	v.run(t)
}

func TestHexadecimalFloat(t *testing.T) {
	v := SingleTests{
		{"0x1. ", item{token.ILLEGAL, "0x1."}, errHexFloatNoExponent},
		{"0xp ", item{token.ILLEGAL, "0xp"}, errFloatExponentHasNoDigits},
		{"0xp2 ", item{token.ILLEGAL, "0xp2"}, errNumberHasNodigits[base16]},
		{"0xfp ", item{token.ILLEGAL, "0xfp"}, errFloatExponentHasNoDigits},
		{"0xfp3 ", item{token.FLOAT, "0xfp3"}, nil},
		{"0x.p3 ", item{token.ILLEGAL, "0x.p3"}, errNumberHasNodigits[base16]},
		{"0x.0p3 ", item{token.FLOAT, "0x.0p3"}, nil},
		{"0x0.p-99 ", item{token.FLOAT, "0x0.p-99"}, nil},
		{"0x1234_5678_9abc.def0_123p-123_456 ", item{token.FLOAT, "0x1234_5678_9abc.def0_123p-123_456"}, nil},
	}
	v.run(t)
}

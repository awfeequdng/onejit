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

func TestComment(t *testing.T) {
	v := SingleTests{
		{"/", item{token.QUO, ""}, nil},
		{"//", item{token.COMMENT, "//"}, nil},
		{"// x\r\n", item{token.COMMENT, "// x"}, nil},
		{"/*", item{token.COMMENT, "/*"}, nil},
		{"/** /", item{token.COMMENT, "/** /"}, nil},
		{"/**/ ", item{token.COMMENT, "/**/"}, nil},
		{"/*x\ny*/ ", item{token.COMMENT, "/*x\ny*/"}, nil},
	}
	v.run(t)
}

func TestIdentifier(t *testing.T) {
	v := SingleTests{
		// BOM is skipped at start-of-file
		{"\ufeffx", item{token.IDENT, "x"}, nil},
		// BOM is invalid in other positions
		{"\ufeff\ufeff", item{token.ILLEGAL, "\ufeff"}, errInvalidCharacter},
		{"a", item{token.IDENT, "a"}, nil},
		{"_ ", item{token.IDENT, "_"}, nil},
		{"_b ", item{token.IDENT, "_b"}, nil},
		{"\u00e0", item{token.IDENT, "\u00e0"}, nil},               // U+00E0 is a letter
		{"\u00f1+", item{token.IDENT, "\u00f1"}, nil},              // U+00F1 is a letter
		{"foo\U0001e959", item{token.IDENT, "foo\U0001e959"}, nil}, // U+1E959 is a number
		{"break;", item{token.BREAK, ""}, nil},
	}
	v.run(t)
}

func TestOperator(t *testing.T) {
	v := SingleTests{
		{"+", item{token.ADD, ""}, nil},
		{"+-", item{token.ADD, ""}, nil},
		{"++.", item{token.INC, ""}, nil},
		{"@", item{token.ILLEGAL, "@"}, errInvalidCharacter},
		{"&^.", item{token.AND_NOT, ""}, nil},
		{"&^=", item{token.AND_NOT_ASSIGN, ""}, nil},
		{".", item{token.PERIOD, ""}, nil},
		{".~", item{token.PERIOD, ""}, nil},
		{".x", item{token.PERIOD, ""}, nil},
		{"..", item{token.PERIOD, ""}, nil},
		{".. ", item{token.PERIOD, ""}, nil},
		{"..x", item{token.PERIOD, ""}, nil},
		{"...", item{token.ELLIPSIS, ""}, nil},
	}
	v.run(t)
}

func TestRune(t *testing.T) {
	v := SingleTests{
		{"'", item{token.ILLEGAL, "'"}, errStringUnterminated[1]},
		{"''", item{token.ILLEGAL, "''"}, errRuneEmpty},
		{"'''", item{token.ILLEGAL, "''"}, errRuneEmpty},
		{`'x'`, item{token.CHAR, `'x'`}, nil},
		{`'\a'`, item{token.CHAR, `'\a'`}, nil},
		{`'\''`, item{token.CHAR, `'\''`}, nil},
		{`'\"'`, item{token.ILLEGAL, `'\"'`}, errEscapeUnknown},
		{`'ab'`, item{token.ILLEGAL, `'ab'`}, errRuneTooLong},
		{`'\377'`, item{token.CHAR, `'\377'`}, nil},
		{`'\378'`, item{token.ILLEGAL, `'\378'`}, errEscapeOctalInvalidChar},
		{`'\477'`, item{token.ILLEGAL, `'\477'`}, errEscapeInvalidCodepoint},
		{`'\xff'`, item{token.CHAR, `'\xff'`}, nil},
		{`'\xf.'`, item{token.ILLEGAL, `'\xf.'`}, errEscapeHexInvalidChar},
		{`'\x00\n'`, item{token.ILLEGAL, `'\x00\n'`}, errRuneTooLong},
		{`'\uffff'`, item{token.CHAR, `'\uffff'`}, nil},
		{`'\u123'`, item{token.ILLEGAL, `'\u123'`},
			[]string{errEscapeHexInvalidChar, errStringUnterminated[1]},
		},
		{`'\U0010ffff'`, item{token.CHAR, `'\U0010ffff'`}, nil},
		{`'\U0003456'`, item{token.ILLEGAL, `'\U0003456'`},
			[]string{errEscapeHexInvalidChar, errStringUnterminated[1]},
		},
	}
	v.run(t)
}

func TestString(t *testing.T) {
	v := SingleTests{
		{`"`, item{token.ILLEGAL, `"`}, errStringUnterminated[0]},
		{`""`, item{token.STRING, `""`}, nil},
		{`"a b\"c d"`, item{token.STRING, `"a b\"c d"`}, nil},
		{`"\a\b\f\n\r\t\v\"\\"`, item{token.STRING, `"\a\b\f\n\r\t\v\"\\"`}, nil},
		{`"\'"`, item{token.ILLEGAL, `"\'"`}, errEscapeUnknown},
		{"\"abc\n\"", item{token.ILLEGAL, `"abc`}, errStringContainsNewline[0]},
		{`"\377"`, item{token.STRING, `"\377"`}, nil},
		{`"\378"`, item{token.ILLEGAL, `"\378"`}, errEscapeOctalInvalidChar},
		{`"\477"`, item{token.ILLEGAL, `"\477"`}, errEscapeInvalidCodepoint},
		{`"\xff"`, item{token.STRING, `"\xff"`}, nil},
		{`"\xf."`, item{token.ILLEGAL, `"\xf."`}, errEscapeHexInvalidChar},
		{`"\uffff"`, item{token.STRING, `"\uffff"`}, nil},
		{`"\u123"`, item{token.ILLEGAL, `"\u123"`},
			[]string{errEscapeHexInvalidChar, errStringUnterminated[0]},
		},
		{`"\U0010ffff"`, item{token.STRING, `"\U0010ffff"`}, nil},
		{`"\U0003456"`, item{token.ILLEGAL, `"\U0003456"`},
			[]string{errEscapeHexInvalidChar, errStringUnterminated[0]},
		},
	}
	v.run(t)
}

func TestRawString(t *testing.T) {
	v := SingleTests{
		{"`", item{token.ILLEGAL, "`"}, errStringUnterminated[2]},
		{"``", item{token.STRING, "``"}, nil},
		{"`\r`", item{token.STRING, "``"}, nil},     // '\r' is discarded
		{"`\n`", item{token.STRING, "`\n`"}, nil},   // '\n' is allowed
		{"`\\x`", item{token.STRING, "`\\x`"}, nil}, // '\\' is not interpreted
	}
	v.run(t)
}

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
		{"0_1234_567i ", item{token.IMAG, "0_1234_567i"}, nil},
		{"0o", item{token.ILLEGAL, "0o"}, errNumberHasNodigits[base8]},
		{"0o0", item{token.INT, "0o0"}, nil},
		{"0o1 ", item{token.INT, "0o1"}, nil},
		{"0o_1 ", item{token.INT, "0o_1"}, nil},
		{"0o_1_ ", item{token.ILLEGAL, "0o_1_"}, errIntBadUnderscore},
		{"0o__1 ", item{token.ILLEGAL, "0o__"}, errIntBadUnderscore},
		{"0o9 ", item{token.ILLEGAL, "0o9"}, errIntBadDigit[base8]},
		{"0o1234_56701_234567 ", item{token.INT, "0o1234_56701_234567"}, nil},
		{"0o_1234_567i ", item{token.IMAG, "0o_1234_567i"}, nil},
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
		{"1_2i ", item{token.IMAG, "1_2i"}, nil},
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
		{"0x_cdef ", item{token.INT, "0x_cdef"}, nil},
		{"0x_cdefi ", item{token.IMAG, "0x_cdefi"}, nil},
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
		{"0x_1i ", item{token.IMAG, "0x_1i"}, nil},
		{"0x_1234_5678_9abc.def0_123p-123_456 ",
			item{token.FLOAT, "0x_1234_5678_9abc.def0_123p-123_456"}, nil},
	}
	v.run(t)
}

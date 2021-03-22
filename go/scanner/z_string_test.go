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
			[]error{errEscapeHexInvalidChar, errStringUnterminated[1]},
		},
		{`'\U0010ffff'`, item{token.CHAR, `'\U0010ffff'`}, nil},
		{`'\U0003456'`, item{token.ILLEGAL, `'\U0003456'`},
			[]error{errEscapeHexInvalidChar, errStringUnterminated[1]},
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
			[]error{errEscapeHexInvalidChar, errStringUnterminated[0]},
		},
		{`"\U0010ffff"`, item{token.STRING, `"\U0010ffff"`}, nil},
		{`"\U0003456"`, item{token.ILLEGAL, `"\U0003456"`},
			[]error{errEscapeHexInvalidChar, errStringUnterminated[0]},
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

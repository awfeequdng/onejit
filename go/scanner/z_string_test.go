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

func TestString(t *testing.T) {
	v := TestCases{
		TestCase{`"`, Item{token.ILLEGAL, `"`}, errStringUnterminated[0]},
		TestCase{`""`, Item{token.STRING, `""`}, nil},
		TestCase{`"a b\"c d"`, Item{token.STRING, `"a b\"c d"`}, nil},
		TestCase{`"\a\b\f\n\r\t\v\"\\"`, Item{token.STRING, `"\a\b\f\n\r\t\v\"\\"`}, nil},
		TestCase{"\"abc\n\"", Item{token.ILLEGAL, `"abc`}, errStringContainsNewline[0]},
		TestCase{`"\377"`, Item{token.STRING, `"\377"`}, nil},
		TestCase{`"\378"`, Item{token.ILLEGAL, `"\378"`}, errEscapeOctalInvalidChar},
		TestCase{`"\477"`, Item{token.ILLEGAL, `"\477"`}, errEscapeInvalidCodepoint},
		TestCase{`"\xff"`, Item{token.STRING, `"\xff"`}, nil},
		TestCase{`"\xf."`, Item{token.ILLEGAL, `"\xf."`}, errEscapeHexInvalidChar},
		TestCase{`"\uffff"`, Item{token.STRING, `"\uffff"`}, nil},
		TestCase{`"\u123"`, Item{token.ILLEGAL, `"\u123"`},
			[]error{errEscapeHexInvalidChar, errStringUnterminated[0]},
		},
		TestCase{`"\U0010ffff"`, Item{token.STRING, `"\U0010ffff"`}, nil},
		TestCase{`"\U0003456"`, Item{token.ILLEGAL, `"\U0003456"`},
			[]error{errEscapeHexInvalidChar, errStringUnterminated[0]},
		},
	}
	v.run(t)
}

func TestRune(t *testing.T) {
	v := TestCases{
		TestCase{"'", Item{token.ILLEGAL, "'"}, errStringUnterminated[1]},
		TestCase{"''", Item{token.ILLEGAL, "''"}, errRuneEmpty},
		TestCase{"'''", Item{token.ILLEGAL, "''"}, errRuneEmpty},
		TestCase{`'x'`, Item{token.CHAR, `'x'`}, nil},
		TestCase{`'\''`, Item{token.CHAR, `'\''`}, nil},
		TestCase{`'\a'`, Item{token.CHAR, `'\a'`}, nil},
		TestCase{`'ab'`, Item{token.ILLEGAL, `'ab'`}, errRuneTooLong},
		TestCase{`'\377'`, Item{token.CHAR, `'\377'`}, nil},
		TestCase{`'\378'`, Item{token.ILLEGAL, `'\378'`}, errEscapeOctalInvalidChar},
		TestCase{`'\477'`, Item{token.ILLEGAL, `'\477'`}, errEscapeInvalidCodepoint},
		TestCase{`'\xff'`, Item{token.CHAR, `'\xff'`}, nil},
		TestCase{`'\xf.'`, Item{token.ILLEGAL, `'\xf.'`}, errEscapeHexInvalidChar},
		TestCase{`'\x00\n'`, Item{token.ILLEGAL, `'\x00\n'`}, errRuneTooLong},
		TestCase{`'\uffff'`, Item{token.CHAR, `'\uffff'`}, nil},
		TestCase{`'\u123'`, Item{token.ILLEGAL, `'\u123'`},
			[]error{errEscapeHexInvalidChar, errStringUnterminated[1]},
		},
		TestCase{`'\U0010ffff'`, Item{token.CHAR, `'\U0010ffff'`}, nil},
		TestCase{`'\U0003456'`, Item{token.ILLEGAL, `'\U0003456'`},
			[]error{errEscapeHexInvalidChar, errStringUnterminated[1]},
		},
	}
	v.run(t)
}

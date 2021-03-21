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
		TestCase{"\"\"", Item{token.STRING, "\"\""}, nil},
		TestCase{`"a b\"c d"`, Item{token.STRING, `"a b\"c d"`}, nil},
		TestCase{`"\a\b\f\n\r\t\v\"\\"`, Item{token.STRING, `"\a\b\f\n\r\t\v\"\\"`}, nil},
		TestCase{"\"abc\n\"", Item{token.ILLEGAL, `"abc`}, errNewlineInString[0]},
		TestCase{`"`, Item{token.ILLEGAL, `"`}, errSyntaxErrorUnterminatedString[0]},
		TestCase{`"\xff"`, Item{token.STRING, `"\xff"`}, nil},
		TestCase{`"\uffff"`, Item{token.STRING, `"\uffff"`}, nil},
		TestCase{`"\U0010ffff"`, Item{token.STRING, `"\U0010ffff"`}, nil},
		TestCase{`"\xf."`, Item{token.ILLEGAL, `"\xf."`}, errInvalidCharInHexEscape},
		TestCase{`"\u123"`, Item{token.ILLEGAL, `"\u123"`},
			[]error{errInvalidCharInHexEscape, errSyntaxErrorUnterminatedString[0]},
		},
		TestCase{`"\U0003456"`, Item{token.ILLEGAL, `"\U0003456"`},
			[]error{errInvalidCharInHexEscape, errSyntaxErrorUnterminatedString[0]},
		},
	}
	v.run(t)
}

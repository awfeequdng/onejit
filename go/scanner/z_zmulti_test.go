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

func TestMulti(t *testing.T) {
	v := MultiTests{
		{"a + b", []item{{token.IDENT, "a"}, {token.ADD, ""}, {token.IDENT, "b"}}, nil},
		{"1 * 'c' / 3i",
			[]item{{token.INT, "1"}, {token.MUL, ""}, {token.CHAR, "'c'"}, {token.QUO, ""}, {token.IMAG, "3i"}}, nil},
		{"/* comment block */ // comment line \npackage main\nimport ( \"fmt\" )\nfunc main() {}",
			[]item{
				{token.COMMENT, "/* comment block */"},
				{token.COMMENT, "// comment line "},
				{token.PACKAGE, ""}, {token.IDENT, "main"},
				{token.IMPORT, ""}, {token.LPAREN, ""}, {token.STRING, "\"fmt\""}, {token.RPAREN, ""},
				{token.FUNC, ""}, {token.IDENT, "main"}, {token.LPAREN, ""}, {token.RPAREN, ""},
				{token.LBRACE, ""}, {token.RBRACE, ""},
			}, nil},
	}
	v.run(t)
}

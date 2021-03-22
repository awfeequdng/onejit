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
		{"..", item{token.ILLEGAL, ".."}, errUnexpectedEOF},
		{".. ", item{token.ILLEGAL, ".."}, errInvalidOperator},
		{"..x", item{token.ILLEGAL, "..x"}, errInvalidOperator},
		{"...", item{token.ELLIPSIS, ""}, nil},
	}
	v.run(t)
}

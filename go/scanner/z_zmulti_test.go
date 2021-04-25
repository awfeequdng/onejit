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
	"go/build"
	"testing"

	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/testutil"
	"github.com/cosmos72/onejit/go/token"
)

func TestMulti(t *testing.T) {
	semi := item{token.SEMICOLON, ""}
	lparen := item{token.LPAREN, ""}
	rparen := item{token.RPAREN, ""}

	v := MultiTests{
		{"a + b",
			[]item{{token.IDENT, "a"}, {token.ADD, ""}, {token.IDENT, "b"}}, nil},
		{"1 * 'c' / 3i",
			[]item{{token.INT, "1"}, {token.MUL, ""}, {token.CHAR, "'c'"}, {token.QUO, ""}, {token.IMAG, "3i"}}, nil},
		{"(3)", []item{lparen, {token.INT, "3"}, rparen}, nil},
		{"/* comment block */\n\n// comment line \npackage main\n\nimport ( \"fmt\" )\n\nfunc main() {\n}\n\n",
			[]item{
				{token.COMMENT, "/* comment block */"},
				{token.COMMENT, "// comment line "},
				{token.PACKAGE, ""}, {token.IDENT, "main"}, semi,
				{token.IMPORT, ""}, lparen, {token.STRING, "\"fmt\""}, rparen, semi,
				{token.FUNC, ""}, {token.IDENT, "main"}, lparen, rparen,
				{token.LBRACE, ""}, {token.RBRACE, ""}, semi,
			}, nil},
		{"(func ())(nil)",
			[]item{
				lparen, {token.FUNC, ""}, lparen, rparen, rparen,
				lparen, {token.IDENT, "nil"}, rparen,
			}, nil},
		{"int64(0xc008427b)",
			[]item{{token.IDENT, "int64"}, lparen, {token.INT, "0xc008427b"}, rparen}, nil},
	}
	v = MultiTests{
		{"a . \n b",
			[]item{{token.IDENT, "a"}, {token.PERIOD, ""}, {token.IDENT, "b"}}, nil},
	}
	v.run(t)
}

func TestScanGoRootFiles(t *testing.T) {
	s := &Scanner{}
	visit := func(t *testing.T, in io.Reader, filename string) {
		scanFile(t, s, in, filename)
	}
	testutil.VisitFilesRecurse(t, visit, build.Default.GOROOT)
}

func scanFile(t *testing.T, s *Scanner, in io.Reader, filename string) {
	s.ClearErrors()
	s.Init(token.NewFile(filename, 0), in)
	for {
		tok, lit := s.Scan()
		if tok == token.EOF {
			break
		}
		if tok == token.ILLEGAL {
			t.Errorf("scan file %q returned {%v %q}", filename, tok, lit)
		}
	}
	testutil.CompareErrors(t, filename, s.Errors(), nil)
}

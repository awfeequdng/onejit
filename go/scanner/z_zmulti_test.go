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
	"bytes"
	"go/build"
	"io/ioutil"
	"os"
	"path"
	"sort"
	"testing"

	"github.com/cosmos72/onejit/go/token"
)

func TestMulti(t *testing.T) {
	semi := item{token.SEMICOLON, ""}

	v := MultiTests{
		{"a + b", []item{{token.IDENT, "a"}, {token.ADD, ""}, {token.IDENT, "b"}}, nil},
		{"1 * 'c' / 3i",
			[]item{{token.INT, "1"}, {token.MUL, ""}, {token.CHAR, "'c'"}, {token.QUO, ""}, {token.IMAG, "3i"}}, nil},
		{"/* comment block */\n\n// comment line \npackage main\n\nimport ( \"fmt\" )\n\nfunc main() {\n}\n",
			[]item{
				{token.COMMENT, "/* comment block */"},
				{token.COMMENT, "// comment line "},
				{token.PACKAGE, ""}, {token.IDENT, "main"}, semi,
				{token.IMPORT, ""}, {token.LPAREN, ""}, {token.STRING, "\"fmt\""}, {token.RPAREN, ""}, semi,
				{token.FUNC, ""}, {token.IDENT, "main"}, {token.LPAREN, ""}, {token.RPAREN, ""},
				{token.LBRACE, ""}, {token.RBRACE, ""}, semi,
			}, nil},
	}
	v.run(t)
}

func TestGoRootFiles(t *testing.T) {
	testFilesAndDirs(t, &Scanner{}, build.Default.GOROOT)
}

func testFilesAndDirs(t *testing.T, s *Scanner, dirname string) bool {
	if len(dirname) > 512 {
		// try to avoid symlink loops
		t.Skip("path too long, aborting test: ", dirname)
		return false
	}
	d, err := os.Open(dirname)
	if err != nil {
		t.Logf("error opening directory %q, skipping it: %v", dirname, err)
		return true
	}
	info, err := d.Readdir(0)
	d.Close()
	if err != nil {
		t.Logf("error reading directory %q: %v", dirname, err)
		return true
	}
	sortInfo(info)
	for _, entry := range info {
		name := path.Join(dirname, entry.Name())
		if entry.IsDir() {
			if !testFilesAndDirs(t, s, name) {
				return false
			}
		} else if entry.Mode().IsRegular() &&
			len(name) > 3 && name[len(name)-3:] == ".go" {

			testFile(t, s, name)
		}
	}
	return true
}

func sortInfo(info []os.FileInfo) {
	sort.Slice(info, func(i int, j int) bool {
		return info[i].Name() < info[j].Name()
	})
}

func testFile(t *testing.T, s *Scanner, filename string) {
	f, err := os.Open(filename)
	if err != nil {
		t.Logf("error opening file %q, skipping it: %v", filename, err)
		return
	}
	defer f.Close()
	b, _ := ioutil.ReadAll(f)
	if bytes.Contains(b, []byte("ERROR")) {
		// file is supposed to contain some kind of error, skip it
		return
	}
	f.Seek(0, 0)
	s.Init(token.NewFile(filename, 0), f)
	for {
		tok, lit := s.Scan()
		if tok == token.EOF {
			break
		}
		if tok == token.ILLEGAL {
			t.Errorf("scan file %q returned {%v %q}", filename, tok, lit)
		}
	}
	compareErrors(t, filename, s.Errors(), nil)
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_multi_test.go
 *
 *  Created on: Mar 28, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"go/build"
	"io"
	"testing"

	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/testutil"
	"github.com/cosmos72/onejit/go/token"
)

func disabled_TestGoRootFiles(t *testing.T) {
	s := &scanner.Scanner{}
	p := &Parser{}
	visit := func(t *testing.T, in io.Reader, filename string) {
		parseFile(t, s, p, in, filename)
	}
	testutil.RecursiveVisitDir(t, visit, build.Default.GOROOT)
}

func parseFile(t *testing.T, s *scanner.Scanner, p *Parser, in io.Reader, filename string) {
	s.Init(token.NewFile(filename, 0), in)
	p.Init(s, Default)
	for {
		node := p.Parse()
		if tok := node.Op(); tok == token.EOF {
			break
		} else if tok == token.ILLEGAL {
			t.Errorf("parse file %q returned %v", filename, node)
		}
	}
	testutil.CompareErrors(t, filename, stringList{p.Errors()}, nil)
}

type stringList struct {
	errors *[]*scanner.Error
}

func (list stringList) Len() int {
	return len(*list.errors)
}

func (list stringList) At(i int) string {
	return (*list.errors)[i].Msg
}

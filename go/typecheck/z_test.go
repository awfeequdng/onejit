/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_test.go
 *
 *  Created on: Apr 06, 2021
 *      Author: Massimiliano Ghilardi
 */

package typecheck

import (
	"go/build"
	"io"
	"testing"

	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/parser"
	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/testutil"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

func parse(p *parser.Parser, t *testing.T) (nodes []ast.Node) {
	for {
		node := p.Parse()
		if node == nil {
			continue
		} else if tok := node.Op(); tok == token.EOF {
			break
		} else if tok == token.ILLEGAL {
			t.Errorf("parse returned %v", node)
		}
		nodes = append(nodes, node)
	}
	testutil.CompareErrors(t, "test.go", errorList{p.Errors()}, nil)
	return nodes
}

type errorList struct {
	errors *[]*scanner.Error
}

func (list errorList) Len() int {
	return len(*list.errors)
}

func (list errorList) String(i int) string {
	return (*list.errors)[i].Msg
}

func (list errorList) Error(i int) error {
	return (*list.errors)[i]
}

func TestChecker(t *testing.T) {
	var p parser.Parser
	p.InitString("const ( a = 1; b = 2 )", parser.Default)
	CheckGlobals(types.Universe(), nil, p.Parse())
}

func TestCheckGoRootFiles(t *testing.T) {
	var p parser.Parser
	var c Checker
	visit := func(t *testing.T, in io.Reader, filename string) {
		p.Init(token.NewFile(filename, 0), in, parser.Default)
		testutil.CompareErrors(t, filename, errorList{p.Errors()}, nil)

		c.Init(types.NewScope(types.Universe()), nil)
		c.CheckGlobals(p.ParseFile())
		if false /*testing.Verbose()*/ {
			t.Log(strings.Basename(filename), ":", c.globals)
		}
	}
	testutil.VisitDirRecurse(t, visit, build.Default.GOROOT)
}

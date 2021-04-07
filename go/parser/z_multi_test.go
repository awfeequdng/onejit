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
	"testing"

	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/testutil"
	"github.com/cosmos72/onejit/go/token"
)

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

func parseFromString(t *testing.T, p *Parser, source string) {
	p.InitString(source, Default)
	for {
		node := p.Parse()
		if node == nil {
			continue
		} else if tok := node.Op(); tok == token.EOF {
			break
		} else if tok == token.ILLEGAL {
			t.Errorf("parse from string returned %v", node)
		}
	}
	testutil.CompareErrors(t, "<string>", errorList{p.Errors()}, nil)
}

func TestBuiltinFunctions(t *testing.T) {
	p := &Parser{}
	source := "func append(slice []Type, elems ...Type) []Type\nfunc copy(dst, src []Type) int"
	parseFromString(t, p, source)
}

func TestParseGoRootFiles(t *testing.T) {
	p := Parser{}
	visit := func(t *testing.T, in io.Reader, filename string) {
		p.Init(token.NewFile(filename, 0), in, Go1_9)
		p.ParseFile()
		testutil.CompareErrors(t, filename, errorList{p.Errors()}, nil)
	}
	testutil.VisitDirRecurse(t, visit, build.Default.GOROOT)
}

func TestParseOnejitGoFiles(t *testing.T) {
	// t.SkipNow()
	p := Parser{}
	visit := func(t *testing.T, in io.Reader, filename string) {
		p.Init(token.NewFile(filename, 0), in, Go1_9)
		file := p.ParseFile()
		testutil.CompareErrors(t, filename, errorList{p.Errors()}, nil)
		showImports(t, file)
	}
	testutil.VisitDirRecurse(t, visit, "..")
}

func showImports(t *testing.T, file *ast.File) {
	list := file.Imports
	if list == nil {
		return
	}
	for _, imp := range list.Nodes {
		n := imp.Len()
		for i := 0; i < n; i++ {
			t.Log(imp.At(i).At(1).(*ast.Atom).Lit)
		}
	}
}

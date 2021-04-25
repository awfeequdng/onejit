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
	"github.com/cosmos72/onejit/go/sort"
	"github.com/cosmos72/onejit/go/testutil"
	"github.com/cosmos72/onejit/go/token"
)

type errorList struct {
	errors []*token.Error
}

func (list *errorList) Len() int {
	return len(list.errors)
}

func (list *errorList) String(i int) string {
	return (list.errors)[i].Msg
}

func (list *errorList) Error(i int) error {
	return (list.errors)[i]
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
	testutil.CompareErrors(t, "<string>", &errorList{p.Errors()}, nil)
}

func TestBuiltinFunctions(t *testing.T) {
	p := &Parser{}
	source := "func append(slice []Type, elems ...Type) []Type\nfunc copy(dst, src []Type) int"
	parseFromString(t, p, source)
}

func TestParseGoRootDir(t *testing.T) {
	p := Parser{}
	visit := func(t *testing.T, opener FileOpener, dirname string) {
		p.ClearErrors()
		p.InitParseDir(token.NewFileSet(dirname), opener, Go1_9)
		testutil.CompareErrors(t, dirname, &errorList{p.Errors()}, nil)
	}
	testutil.VisitDirRecurse(t, visit, build.Default.GOROOT)
}

func TestParseOnejitGoDir(t *testing.T) {
	// t.SkipNow()
	p := Parser{}
	visit := func(t *testing.T, opener FileOpener, dirname string) {
		p.ClearErrors()
		dir := p.InitParseDir(token.NewFileSet(dirname), opener, Go1_9)
		testutil.CompareErrors(t, dirname, &errorList{p.Errors()}, nil)
		showDirImports(t, dir)
	}
	testutil.VisitDirRecurse(t, visit, "..")
}

func showDirImports(t *testing.T, dir *ast.Dir) {
	m := make(map[string]struct{})
	for _, file := range dir.Files {
		imps := file.Imports
		if imps == nil {
			return
		}
		for _, imp := range imps.Nodes {
			n := imp.Len()
			for i := 0; i < n; i++ {
				str := imp.At(i).At(1).(*ast.Atom).Lit
				m[str] = struct{}{}
			}
		}
	}
	if true {
		return
	}
	list := make([]string, 0, len(m))
	for str := range m {
		list = append(list, str)
	}
	sort.Strings(list)
	t.Log(list)
}

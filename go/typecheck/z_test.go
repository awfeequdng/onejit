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
	"testing"

	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/parser"
	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/testutil"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
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

func TestChecker(t *testing.T) {
	var p parser.Parser
	p.InitString("const ( a = 1; b = 2 )", parser.Default)
	CheckGlobals(nil, types.Universe(), nil, p.Parse())
}

func TestCheckGoRootFiles(t *testing.T) {
	var p parser.Parser
	var c Checker
	visit := func(t *testing.T, in io.Reader, filename string) {
		p.Init(token.NewFile(filename, 0), in, parser.Go1_9)
		testutil.CompareErrors(t, filename, errorList{p.Errors()}, nil)

		c.Init(nil, types.NewScope(types.Universe()), nil)
		c.CheckGlobals(p.ParseFile())
		if false /*testing.Verbose()*/ {
			t.Log(strings.Basename(filename), ":", c.globals)
		}
	}
	testutil.VisitDirRecurse(t, visit, build.Default.GOROOT)
}

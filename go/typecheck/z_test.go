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

	"github.com/cosmos72/onejit/go/parser"
	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/testutil"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

type errorList struct {
	errors []*scanner.Error
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

func TestChecker(t *testing.T) {
	var p parser.Parser
	p.InitString("const ( a = 1; b = 2 )", parser.Default)
	CheckGlobals(nil, types.Universe(), nil, p.Parse())
}

func TestCheckGoRootDir(t *testing.T) {
	var p parser.Parser
	var c Collector
	visit := func(t *testing.T, opener testutil.Opener, dirname string) {
		p.ClearErrors()
		fset := token.NewFileSet(dirname)
		dir := p.InitParseDir(fset, opener, parser.Go1_9)
		testutil.CompareErrors(t, dirname, &errorList{p.Errors()}, nil)

		c.ClearErrors()
		c.ClearWarnings()
		c.Init(fset, types.NewScope(types.Universe()), nil)
		c.Globals(dir)
		if testing.Verbose() {
			dirbasename := strings.Basename(fset.Name())
			t.Log(dirbasename, "decls:", c.scope.Names())
			for file, scope := range c.files {
				t.Log(dirbasename+"/"+strings.Basename(file.Name()), "imports:", scope.Names())
			}
		}
	}
	testutil.VisitDirRecurse(t, visit, build.Default.GOROOT)
}

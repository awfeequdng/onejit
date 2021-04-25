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
	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/testutil"
	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

func TestChecker(t *testing.T) {
	var p parser.Parser
	p.InitString("const ( a = 1; b = 2 )", parser.Default)
	CheckGlobals(nil, types.Universe(), nil, p.Parse())
}

func TestCheckGoRootDir(t *testing.T) {
	visit := makeVisitor(false)
	testutil.VisitDirRecurse(t, visit, build.Default.GOROOT)
}

func TestParseOnejitGoDir(t *testing.T) {
	visit := makeVisitor(false)
	testutil.VisitDirRecurse(t, visit, "..")
}

func TestParseTestdataDir(t *testing.T) {
	visit := makeVisitor(true)
	testutil.VisitDirRecurse(t, visit, "testdata")
}

func makeVisitor(verbose bool) func(t *testing.T, opener testutil.Opener, dirname string) {
	var p parser.Parser
	var c Collector
	var r Resolver
	return func(t *testing.T, opener testutil.Opener, dirname string) {
		p.ClearErrors()
		fset := token.NewFileSet(dirname)
		dir := p.InitParseDir(fset, opener, parser.Go1_9)
		testutil.CompareErrors(t, dirname, p.Errors(), nil)

		c.ClearErrors()
		c.ClearWarnings()
		c.Init(fset, types.NewScope(types.Universe()), nil)
		c.Globals(dir)
		r.Init(&c)
		r.Globals()
		r.DeclareGlobals()
		if verbose && testing.Verbose() {
			dirbasename := strings.Basename(fset.Name())
			for file, syms := range c.fileobjs {
				t.Log(dirbasename+"/"+strings.Basename(file.Name()), "imports:", syms.Names())
			}
			t.Log(dirbasename, "decls:", c.objs.Names())
			t.Log(dirbasename, "resolved:", r.resolved)
		}
	}
}

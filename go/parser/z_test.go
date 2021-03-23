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
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"fmt"
	"strings"
	"testing"

	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/token"
)

func makeParser(str string) *Parser {
	var reader strings.Reader
	var s scanner.Scanner
	var p Parser
	reader.Reset(str)
	s.Init(token.NewFile("test.go", 0), &reader)
	p.Init(&s)
	return &p
}

func compareNode(t *testing.T, node ast.Node, expected string) {
	actual := fmt.Sprint(node)
	if actual != expected {
		t.Errorf("parse returned\n\t%v\nexpecting\n\t%v", actual, expected)
	}
}

func TestPackage(t *testing.T) {
	p := makeParser(`package main`)
	compareNode(t, p.Parse(), `[package (IDENT "main")]`)
}

func TestImport(t *testing.T) {
	p := makeParser(`import _ "fmt"`)
	compareNode(t, p.Parse(), `[import [IMPORT_SPEC (IDENT "_") (STRING "\"fmt\"")]]`)
}

func TestConst(t *testing.T) {
	p := makeParser(`const ( a, b )`)
	compareNode(t, p.Parse(), `[const [SPEC [IDENT_LIST (IDENT "a") (IDENT "b")] nil nil]]`)
}

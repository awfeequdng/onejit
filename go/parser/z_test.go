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

func makeParser(str string) (*Parser, *strings.Reader) {
	var reader strings.Reader
	var s scanner.Scanner
	var p Parser
	reader.Reset(str)
	s.Init(token.NewFile("test.go", 0), &reader)
	p.Init(&s)
	return &p, &reader
}

func compareNode(t *testing.T, node ast.Node, expected string) {
	actual := fmt.Sprint(node)
	if actual != expected {
		t.Errorf("parse returned\n\t%v\nexpecting\n\t%v", actual, expected)
	}
}

func TestPackage(t *testing.T) {
	p, _ := makeParser(`package main`)
	compareNode(t, p.Parse(), `[package (IDENT "main")]`)
}

func TestImport(t *testing.T) {
	p, _ := makeParser(`import _ "fmt"`)
	compareNode(t, p.Parse(), `[import [IMPORT_SPEC (IDENT "_") (STRING "\"fmt\"")]]`)
}

func TestConst(t *testing.T) {
	p, _ := makeParser(`const a, b`)
	compareNode(t, p.Parse(), `[const [VALUE_SPEC [IDENTS (IDENT "a") (IDENT "b")] nil nil]]`)
}

func TestConstTyped(t *testing.T) {
	p, _ := makeParser(`const ( foo, bar *map[int]int )`)
	compareNode(t, p.Parse(), `[const [VALUE_SPEC [IDENTS (IDENT "foo") (IDENT "bar")]`+
		` [* [map (IDENT "int") (IDENT "int")]]`+ // type
		` nil]]`) // expr list
}

func TestVar(t *testing.T) {
	p, _ := makeParser(`var a, b`)
	compareNode(t, p.Parse(), `[var [VALUE_SPEC [IDENTS (IDENT "a") (IDENT "b")] nil nil]]`)
}

func TestVarTyped(t *testing.T) {
	p, _ := makeParser(`var ( foo, bar <-chan <-chan int )`)
	compareNode(t, p.Parse(), `[var [VALUE_SPEC [IDENTS (IDENT "foo") (IDENT "bar")]`+
		` [chan (RECV_DIR) [chan (RECV_DIR) (IDENT "int")]]`+ // type
		` nil]]`) // expr list
}

func TestType(t *testing.T) {
	p, _ := makeParser("type ( Int int\n Rune rune )")
	compareNode(t, p.Parse(), `[type`+
		` [:= (IDENT "Int") (IDENT "int")]`+
		` [:= (IDENT "Rune") (IDENT "rune")]]`)
}

func TestTypeAlias(t *testing.T) {
	p, _ := makeParser("type Byte = byte")
	compareNode(t, p.Parse(), `[type [= (IDENT "Byte") (IDENT "byte")]]`)
}

func TestTypeStruct(t *testing.T) {
	p, _ := makeParser("type tuple struct { a, b int\nc float; e chan int } ")
	compareNode(t, p.Parse(), `[type [:= (IDENT "tuple") [struct`+
		` [FIELD [IDENTS (IDENT "a") (IDENT "b")] (IDENT "int") nil]`+
		` [FIELD [IDENTS (IDENT "c")] (IDENT "float") nil]`+
		` [FIELD [IDENTS (IDENT "e")] [chan nil (IDENT "int")] nil]]]]`)
}

func TestTypeStructTag(t *testing.T) {
	p, _ := makeParser(`type quirky = struct { int int "tag" } `) // field has name "int" and type "int"
	compareNode(t, p.Parse(), `[type [= (IDENT "quirky") [struct`+
		` [FIELD [IDENTS (IDENT "int")] (IDENT "int") (STRING "\"tag\"")]]]]`)
}

func TestTypeStructEmbed(t *testing.T) {
	p, _ := makeParser(`type embed = struct { int; *float } `) // two embedded fields
	compareNode(t, p.Parse(), `[type [= (IDENT "embed") [struct`+
		` [FIELD nil (IDENT "int") nil]`+
		` [FIELD nil [* (IDENT "float")] nil]]]]`)
}

func TestTypeInterface(t *testing.T) {
	p, _ := makeParser(`type fooer interface { Foo(int) } `)
	compareNode(t, p.Parse(), `[type [:= (IDENT "fooer") [interface`+
		` [FIELD nil [func [PARAMS [FIELD nil (IDENT "int") nil]] nil] nil]]]]`)
}

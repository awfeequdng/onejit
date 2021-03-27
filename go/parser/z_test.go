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
	p.Init(&s, Default)
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
	compareNode(t, p.Parse(), `(package (IDENT "main"))`)
}

func TestImport(t *testing.T) {
	p, _ := makeParser(`import _ "fmt"`)
	compareNode(t, p.Parse(), `(import (IMPORT_SPEC (IDENT "_") (STRING "\"fmt\"")))`)
}

func TestDeclConst(t *testing.T) {
	p, _ := makeParser(`const a, b`)
	compareNode(t, p.Parse(), `(const (VALUE_SPEC (NAMES (IDENT "a") (IDENT "b")) nil nil))`)
}

func TestDeclConstTyped(t *testing.T) {
	p, _ := makeParser(`const ( foo, bar *map[int]int )`)
	compareNode(t, p.Parse(), `(const (VALUE_SPEC (NAMES (IDENT "foo") (IDENT "bar"))`+
		` (* (map (IDENT "int") (IDENT "int")))`+ // type
		` nil))`) // expr list
}

func TestDeclVar(t *testing.T) {
	p, _ := makeParser(`var a, b`)
	compareNode(t, p.Parse(), `(var (VALUE_SPEC (NAMES (IDENT "a") (IDENT "b")) nil nil))`)
}

func TestDeclVarTyped(t *testing.T) {
	p, _ := makeParser(`var ( foo, bar <-chan <-chan int )`)
	compareNode(t, p.Parse(), `(var (VALUE_SPEC (NAMES (IDENT "foo") (IDENT "bar"))`+
		` (chan (RECV_DIR) (chan (RECV_DIR) (IDENT "int")))`+ // type
		` nil))`) // expr list
}

func TestDeclType(t *testing.T) {
	p, _ := makeParser("type ( Int int\n Rune rune )")
	compareNode(t, p.Parse(), `(type`+
		` (:= (IDENT "Int") (IDENT "int"))`+
		` (:= (IDENT "Rune") (IDENT "rune")))`)
}

func TestDeclTypeAlias(t *testing.T) {
	p, _ := makeParser("type Byte = byte")
	compareNode(t, p.Parse(), `(type (= (IDENT "Byte") (IDENT "byte")))`)
}

func TestDeclTypeStruct(t *testing.T) {
	p, _ := makeParser("type tuple struct { a, b int\nc float; e chan int } ")
	compareNode(t, p.Parse(), `(type (:= (IDENT "tuple") (struct`+
		` (FIELD (NAMES (IDENT "a") (IDENT "b")) (IDENT "int"))`+
		` (FIELD (NAMES (IDENT "c")) (IDENT "float"))`+
		` (FIELD (NAMES (IDENT "e")) (chan nil (IDENT "int"))))))`)
}

func TestDeclTypeStructTag(t *testing.T) {
	p, _ := makeParser(`type quirky = struct { int int "tag" } `) // field has name "int" and type "int"
	compareNode(t, p.Parse(), `(type (= (IDENT "quirky") (struct`+
		` (FIELD (NAMES (IDENT "int")) (IDENT "int") (STRING "\"tag\"")))))`)
}

func TestDeclTypeStructEmbed(t *testing.T) {
	p, _ := makeParser(`type embed = struct { int; *float } `) // two embedded fields
	compareNode(t, p.Parse(), `(type (= (IDENT "embed") (struct`+
		` (FIELD nil (IDENT "int"))`+
		` (FIELD nil (* (IDENT "float"))))))`)
}

func TestDeclTypeFunc(t *testing.T) {
	p, _ := makeParser(`type StateMachine func(env *Env) (next StateMachine, newEnv *Env)`)
	compareNode(t, p.Parse(), `(type (:= (IDENT "StateMachine") (func `+
		`(PARAMS`+
		` (FIELD (NAMES (IDENT "env")) (* (IDENT "Env")))) `+
		`(RESULTS`+
		` (FIELD (NAMES (IDENT "next")) (IDENT "StateMachine"))`+
		` (FIELD (NAMES (IDENT "newEnv")) (* (IDENT "Env")))))))`)
}

func TestDeclTypeFuncImpliedParamType(t *testing.T) {
	p, _ := makeParser(`type add3 func(a int, b, c int) int`)
	compareNode(t, p.Parse(), `(type (:= (IDENT "add3") (func `+
		`(PARAMS`+
		` (FIELD (NAMES (IDENT "a")) (IDENT "int"))`+
		` (FIELD (NAMES (IDENT "b") (IDENT "c")) (IDENT "int"))) `+
		`(RESULTS`+
		` (FIELD nil (IDENT "int"))))))`)
}

func TestDeclTypeFuncVariadic(t *testing.T) {
	p, _ := makeParser(`type addv func(n0 int, nv... int) int`)
	compareNode(t, p.Parse(), `(type (:= (IDENT "addv") (func `+
		`(PARAMS`+
		` (FIELD (NAMES (IDENT "n0")) (IDENT "int"))`+
		` (FIELD (NAMES (IDENT "nv")) (... (IDENT "int")))) `+
		`(RESULTS`+
		` (FIELD nil (IDENT "int"))))))`)
}

func TestDeclTypeGeneric(t *testing.T) {
	p, _ := makeParser(`type pair[T1, T2 any] struct { first T1; second T2 }`)
	compareNode(t, p.Parse(), `(type (:= (IDENT "pair") (GENERIC `+
		`(PARAMS`+
		` (FIELD (NAMES (IDENT "T1") (IDENT "T2")) (IDENT "any"))) `+
		`(struct`+
		` (FIELD (NAMES (IDENT "first")) (IDENT "T1"))`+
		` (FIELD (NAMES (IDENT "second")) (IDENT "T2"))))))`)
}

func TestDeclTypeInterface(t *testing.T) {
	p, _ := makeParser("type fooer interface { Foo(int)\nBar } ")
	compareNode(t, p.Parse(), `(type (:= (IDENT "fooer") (interface`+
		` (FIELD (NAMES (IDENT "Foo")) (func (PARAMS (FIELD nil (IDENT "int"))) nil))`+
		` (FIELD nil (IDENT "Bar")))))`)
}

func TestDeclFunc(t *testing.T) {
	p, _ := makeParser(`func swap(a,b float) (float float) { } `)
	compareNode(t, p.Parse(), `(func nil (IDENT "swap") (func `+
		`(PARAMS`+
		` (FIELD (NAMES (IDENT "a") (IDENT "b")) (IDENT "float"))) `+
		`(RESULTS`+
		` (FIELD (NAMES (IDENT "float")) (IDENT "float")))) `+
		`(BLOCK))`)
}

func TestDeclFuncBad1(t *testing.T) {
	p, _ := makeParser(`func bad1(a ... float, b float) { } `)
	compareNode(t, p.Parse(), `(func nil (IDENT "bad1") (func `+
		`(PARAMS`+
		` (FIELD (NAMES (IDENT "a")) (Bad (... (IDENT "float"))))`+
		` (FIELD (NAMES (IDENT "b")) (IDENT "float"))) `+
		`nil) `+ // results
		`(BLOCK))`)
}

func TestDeclFuncBad2(t *testing.T) {
	p, _ := makeParser(`func bad2(a +int) { } `)
	compareNode(t, p.Parse(), `(func nil (IDENT "bad2") (func `+
		`(PARAMS`+
		` (FIELD (NAMES (IDENT "a")) (Bad (+)))`+
		` (Bad (IDENT "int"))) `+
		`nil) `+ // results
		`(BLOCK))`)
}

func TestStmtBlock1(t *testing.T) {
	p, _ := makeParser(`{ a; b; c }`)
	compareNode(t, p.Parse(), `(BLOCK (IDENT "a") (IDENT "b") (IDENT "c"))`)
}

func TestStmtBlock2(t *testing.T) {
	p, _ := makeParser(`{ return a, b, c }`)
	compareNode(t, p.Parse(), `(BLOCK (return (IDENT "a") (IDENT "b") (IDENT "c")))`)
}

func TestStmtFor(t *testing.T) {
	p, _ := makeParser(`for { }`)
	compareNode(t, p.Parse(), `(for nil nil nil (BLOCK))`)
}

func TestStmtForCond(t *testing.T) {
	p, _ := makeParser(`for a == b { }`)
	compareNode(t, p.Parse(), `(for nil (== (IDENT "a") (IDENT "b")) nil (BLOCK))`)
}

func TestStmtForInitCondPost(t *testing.T) {
	p, _ := makeParser(`for init; cond; post { }`)
	compareNode(t, p.Parse(), `(for (IDENT "init") (IDENT "cond") (IDENT "post") (BLOCK))`)
}

func TestStmtIf(t *testing.T) {
	p, _ := makeParser(`if a { b } else if c { d }`)
	compareNode(t, p.Parse(), `(if nil (IDENT "a") (BLOCK (IDENT "b")) (if nil (IDENT "c") (BLOCK (IDENT "d")) nil))`)
}

func TestExprParen(t *testing.T) {
	p, _ := makeParser(`((((((((((9))))))))))`)
	compareNode(t, p.Parse(), `(INT "9")`)
}

func TestExpr1(t *testing.T) {
	p, _ := makeParser(`a + b`)
	compareNode(t, p.Parse(), `(+ (IDENT "a") (IDENT "b"))`)
}

func TestExpr2(t *testing.T) {
	p, _ := makeParser(`a + b * c + d`)
	compareNode(t, p.Parse(), `(+ (+ (IDENT "a") (* (IDENT "b") (IDENT "c"))) (IDENT "d"))`)
}

func TestExpr3(t *testing.T) {
	p, _ := makeParser("(a + b) + *c * <-d")
	compareNode(t, p.Parse(), `(+ (+ (IDENT "a") (IDENT "b")) (* (* (IDENT "c")) (<- (IDENT "d"))))`)
}

func TestExprCall(t *testing.T) {
	p, _ := makeParser("a.b(^c, -d)")
	compareNode(t, p.Parse(), `(CALL (. (IDENT "a") (IDENT "b")) (^ (IDENT "c")) (- (IDENT "d")))`)
}

func TestExprIndex1(t *testing.T) {
	p, _ := makeParser("a[b]")
	compareNode(t, p.Parse(), `(INDEX (IDENT "a") (IDENT "b"))`)
}

func TestExprIndex2(t *testing.T) {
	p, _ := makeParser("a[b, c]")
	compareNode(t, p.Parse(), `(INDEX (IDENT "a") (IDENT "b") (IDENT "c"))`)
}

func TestExprSlice0(t *testing.T) {
	p, _ := makeParser("a[:]")
	compareNode(t, p.Parse(), `(SLICE (IDENT "a") nil nil)`)
}

func TestExprSlice1(t *testing.T) {
	p, _ := makeParser("a[b:]")
	compareNode(t, p.Parse(), `(SLICE (IDENT "a") (IDENT "b") nil)`)
}

func TestExprSlice2(t *testing.T) {
	p, _ := makeParser("a[:b]")
	compareNode(t, p.Parse(), `(SLICE (IDENT "a") nil (IDENT "b"))`)
}

func TestExprSlice3(t *testing.T) {
	p, _ := makeParser("a[b::c]")
	compareNode(t, p.Parse(), `(SLICE (IDENT "a") (IDENT "b") nil (IDENT "c"))`)
}

func TestExprConversion(t *testing.T) {
	p, _ := makeParser("(func (int) int)(nil)")
	// parsed as a one-argument call, with fun = "func (int) int" and expr0 = "nil"
	compareNode(t, p.Parse(), `(CALL (func (PARAMS (FIELD nil (IDENT "int"))) (RESULTS (FIELD nil (IDENT "int")))) (IDENT "nil"))`)
}

func TestExprLambda(t *testing.T) {
	p, _ := makeParser("(func (n int) int { return n })(7)")
	compareNode(t, p.Parse(), `(CALL (LAMBDA (func `+
		`(PARAMS`+
		` (FIELD (NAMES (IDENT "n")) (IDENT "int"))) `+
		`(RESULTS`+
		` (FIELD nil (IDENT "int")))) `+
		`(BLOCK`+
		` (return (IDENT "n")))) `+
		// call arguments
		`(INT "7"))`)
}

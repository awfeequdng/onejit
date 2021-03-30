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

func TestDeclVarInit(t *testing.T) {
	p, _ := makeParser(`var ( foo, bar = f1, f2 )`)
	compareNode(t, p.Parse(), `(var (VALUE_SPEC`+
		` (NAMES (IDENT "foo") (IDENT "bar"))`+ // ident list
		` nil`+ // type
		` (EXPRS (IDENT "f1") (IDENT "f2"))))`) // expr list
}

func TestDeclVarTyped(t *testing.T) {
	p, _ := makeParser(`var ( foo, bar <-chan <-chan int )`)
	compareNode(t, p.Parse(), `(var (VALUE_SPEC`+
		` (NAMES (IDENT "foo") (IDENT "bar"))`+ // ident list
		` (chan (RECV_DIR) (chan (RECV_DIR) (IDENT "int")))`+ // type
		` nil))`) // expr list
}

func TestDeclVarTypedInit(t *testing.T) {
	p, _ := makeParser(`var ( x, y, z type1 = e1; w type2 = e2; )`)
	compareNode(t, p.Parse(), `(var `+
		`(VALUE_SPEC`+
		` (NAMES (IDENT "x") (IDENT "y") (IDENT "z"))`+ // ident list
		` (IDENT "type1")`+ // type
		` (EXPRS (IDENT "e1"))) `+
		`(VALUE_SPEC`+
		` (NAMES (IDENT "w"))`+ // ident list
		` (IDENT "type2")`+ // type
		` (EXPRS (IDENT "e2"))))`) // expr list
}

func TestDeclVarTypedInitCompositeLit(t *testing.T) {
	p, _ := makeParser(`var x typ = typ{a:1, b:2}`)
	compareNode(t, p.Parse(), `(var `+
		`(VALUE_SPEC (NAMES (IDENT "x")) (IDENT "typ")`+
		` (EXPRS (COMPOSITE_LIT (IDENT "typ") (KEY_VALUE (IDENT "a") (INT "1")) (KEY_VALUE (IDENT "b") (INT "2"))))))`)
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

func TestDeclTypeArray(t *testing.T) {
	p, _ := makeParser("type Vec [7]uintptr")
	compareNode(t, p.Parse(), `(type (:= (IDENT "Vec") (ARRAY (INT "7") (IDENT "uintptr"))))`)
}

func TestDeclTypeSlice(t *testing.T) {
	p, _ := makeParser("type Slice []complex64")
	compareNode(t, p.Parse(), `(type (:= (IDENT "Slice") (ARRAY nil (IDENT "complex64"))))`)
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

func TestDeclTypeInterface1(t *testing.T) {
	p, _ := makeParser("type any interface {} ")
	compareNode(t, p.Parse(), `(type (:= (IDENT "any") (interface)))`)
}

func TestDeclTypeInterface2(t *testing.T) {
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
		` (FIELD nil (IDENT "a")) (Bad (+))`+
		` (Bad (IDENT "int"))) `+
		`nil) `+ // results
		`(BLOCK))`)
}

func TestDeclFuncEllipsis(t *testing.T) {
	p, _ := makeParser("func println(args ... interface{}) (int, error) { }")
	compareNode(t, p.Parse(), `(func nil (IDENT "println") `+
		`(func`+
		` (PARAMS (FIELD (NAMES (IDENT "args")) (... (interface))))`+
		` (RESULTS (FIELD nil (IDENT "int")) (FIELD nil (IDENT "error")))) `+
		`(BLOCK))`)
}

func TestDeclFuncArgIsArray(t *testing.T) {
	// very tricky, 'args[len]' may also be a generic type instantiation
	// => calls Parser.fixParamDecl() to correct
	p, _ := makeParser("func tostring(args[len]int) { }")
	compareNode(t, p.Parse(), `(func nil (IDENT "tostring") `+
		`(func`+
		` (PARAMS (FIELD (NAMES (IDENT "args")) (ARRAY (IDENT "len") (IDENT "int"))))`+
		` nil) `+
		`(BLOCK))`)
}

func TestDeclFuncArgIsSlice(t *testing.T) {
	// tricky, 'args[' may also start a generic type instantiation
	// => calls Parser.unread() to backtrack
	p, _ := makeParser("func tostring(args []int) { }")
	compareNode(t, p.Parse(), `(func nil (IDENT "tostring") `+
		`(func`+
		` (PARAMS (FIELD (NAMES (IDENT "args")) (ARRAY nil (IDENT "int"))))`+
		` nil) `+
		`(BLOCK))`)
}

func TestDeclFuncArgIsGeneric(t *testing.T) {
	p, _ := makeParser("func tostring(pair[int,uint]) { }")
	compareNode(t, p.Parse(), `(func nil (IDENT "tostring") `+
		`(func`+
		` (PARAMS (FIELD nil (INDEX (IDENT "pair") (IDENT "int") (IDENT "uint"))))`+
		` nil) `+
		`(BLOCK))`)
}

func TestDeclFuncArgIsFunc(t *testing.T) {
	p, _ := makeParser("func foo(func(), error) { }")
	compareNode(t, p.Parse(), `(func nil (IDENT "foo") `+
		`(func`+
		` (PARAMS (FIELD nil (func (PARAMS) nil)) (FIELD nil (IDENT "error")))`+
		` nil) `+
		`(BLOCK))`)
}

func TestDeclFuncResultIsFunc(t *testing.T) {
	p, _ := makeParser("func foo(error) func() { }")
	compareNode(t, p.Parse(), `(func nil (IDENT "foo") `+
		`(func`+
		` (PARAMS (FIELD nil (IDENT "error")))`+
		` (RESULTS (FIELD nil (func (PARAMS) nil)))) `+
		`(BLOCK))`)
}

func TestDeclMethodResults(t *testing.T) {
	p, _ := makeParser(`func (p *Parser) fixParams() (a, b.c, d) { } `)
	compareNode(t, p.Parse(), `(func `+
		`(PARAMS (FIELD (NAMES (IDENT "p")) (* (IDENT "Parser")))) `+ // receiver
		`(IDENT "fixParams") `+ // name
		`(func (PARAMS) `+ // params
		`(RESULTS`+
		` (FIELD nil (IDENT "a"))`+
		` (FIELD nil (. (IDENT "b") (IDENT "c")))`+
		` (FIELD nil (IDENT "d")))) `+
		`(BLOCK))`)
}

func TestExpr0(t *testing.T) {
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

func TestExprCallEllipsis(t *testing.T) {
	p, _ := makeParser("f(a, b...)")
	compareNode(t, p.Parse(), `(CALL (IDENT "f") (IDENT "a") (... (IDENT "b")))`)
}

func TestExprCompositeLit1(t *testing.T) {
	p, _ := makeParser(`T{key1:value1, {key2}:{value2}, value3}`)
	compareNode(t, p.Parse(), `(COMPOSITE_LIT `+
		`(IDENT "T")`+
		` (KEY_VALUE (IDENT "key1") (IDENT "value1"))`+
		` (KEY_VALUE (COMPOSITE_LIT nil (IDENT "key2")) (COMPOSITE_LIT nil (IDENT "value2")))`+
		` (IDENT "value3"))`)
}

func TestExprCompositeLit2(t *testing.T) {
	p, _ := makeParser("struct{ i int }{1}")
	compareNode(t, p.Parse(), `(COMPOSITE_LIT `+
		`(struct (FIELD (NAMES (IDENT "i")) (IDENT "int"))) `+
		`(INT "1"))`)
}

func TestExprIndex1(t *testing.T) {
	p, _ := makeParser("a[b][c]")
	compareNode(t, p.Parse(), `(INDEX (INDEX (IDENT "a") (IDENT "b")) (IDENT "c"))`)
}

func TestExprIndex2(t *testing.T) {
	p, _ := makeParser("a[b, c][d]")
	compareNode(t, p.Parse(), `(INDEX (INDEX (IDENT "a") (IDENT "b") (IDENT "c")) (IDENT "d"))`)
}

func TestExprPeriod(t *testing.T) {
	p, _ := makeParser("a.b().c()")
	compareNode(t, p.Parse(), `(CALL (. (CALL (. (IDENT "a") (IDENT "b"))) (IDENT "c")))`)
}

func TestExprRecv(t *testing.T) {
	p, _ := makeParser(`<- channel`)
	compareNode(t, p.Parse(), `(<- (IDENT "channel"))`)
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

func TestExprConversionFunc(t *testing.T) {
	p, _ := makeParser("(func (int) int)(nil)")
	// parsed as a one-argument call, with fun = "func (int) int" and expr0 = "nil"
	compareNode(t, p.Parse(), `(CALL (func (PARAMS (FIELD nil (IDENT "int"))) (RESULTS (FIELD nil (IDENT "int")))) (IDENT "nil"))`)
}

func TestExprConversionInt64(t *testing.T) {
	p, _ := makeParser("int64(0xc008427b)")
	compareNode(t, p.Parse(), `(CALL (IDENT "int64") (INT "0xc008427b"))`)
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

func TestStmtAssign1(t *testing.T) {
	p, _ := makeParser(`a,b = f()`)
	compareNode(t, p.Parse(), `(= (EXPRS (IDENT "a") (IDENT "b")) (EXPRS (CALL (IDENT "f"))))`)
}

func TestStmtAssign2(t *testing.T) {
	p, _ := makeParser(`a,b := c,d`)
	compareNode(t, p.Parse(), `(:= (EXPRS (IDENT "a") (IDENT "b")) (EXPRS (IDENT "c") (IDENT "d")))`)
}

func TestStmtAssignCompositeLit(t *testing.T) {
	p, _ := makeParser("a = struct{ i int }{1}")
	compareNode(t, p.Parse(), `(= `+
		`(EXPRS (IDENT "a")) `+
		`(EXPRS (COMPOSITE_LIT`+
		` (struct (FIELD (NAMES (IDENT "i")) (IDENT "int")))`+
		` (INT "1"))))`)
}

func TestStmtBlock1(t *testing.T) {
	p, _ := makeParser(`{ a; b; c }`)
	compareNode(t, p.Parse(), `(BLOCK (IDENT "a") (IDENT "b") (IDENT "c"))`)
}

func TestStmtBlock2(t *testing.T) {
	p, _ := makeParser(`{ return a, b, c }`)
	compareNode(t, p.Parse(), `(BLOCK (return (IDENT "a") (IDENT "b") (IDENT "c")))`)
}

func TestStmtDefer(t *testing.T) {
	p, _ := makeParser(`defer obj.mtd(a, b)`)
	compareNode(t, p.Parse(), `(defer (CALL (. (IDENT "obj") (IDENT "mtd")) (IDENT "a") (IDENT "b")))`)
}

func TestStmtFor(t *testing.T) {
	p, _ := makeParser(`for { }`)
	compareNode(t, p.Parse(), `(for nil nil nil (BLOCK))`)
}

func TestStmtForCond(t *testing.T) {
	p, _ := makeParser(`for a == b { }`)
	compareNode(t, p.Parse(), `(for nil (== (IDENT "a") (IDENT "b")) nil (BLOCK))`)
}

func TestStmtForInitCond(t *testing.T) {
	p, _ := makeParser(`for init(); cond(); { }`)
	compareNode(t, p.Parse(), `(for`+
		` (CALL (IDENT "init"))`+
		` (CALL (IDENT "cond"))`+
		` nil `+
		`(BLOCK))`)
}

func TestStmtForInitCondPost(t *testing.T) {
	p, _ := makeParser(`for i := init; i != 0; i-- { break; continue }`)
	compareNode(t, p.Parse(), `(for`+
		` (:= (EXPRS (IDENT "i")) (EXPRS (IDENT "init")))`+
		` (!= (IDENT "i") (INT "0"))`+
		` (-- (IDENT "i")) `+
		`(BLOCK`+
		` (break nil)`+
		` (continue nil)))`)
}

func TestStmtForInitCondPostCompositeLit(t *testing.T) {
	p, _ := makeParser(`for init; cond; (post{key:value}) {  }`)
	compareNode(t, p.Parse(), `(for`+
		` (IDENT "init")`+
		` (IDENT "cond")`+
		` (COMPOSITE_LIT (IDENT "post") (KEY_VALUE (IDENT "key") (IDENT "value"))) `+
		`(BLOCK))`)
}

func TestStmtForRange(t *testing.T) {
	p, _ := makeParser(`for k, v := range m { }`)
	compareNode(t, p.Parse(), `(range (:=`+
		` (EXPRS (IDENT "k") (IDENT "v"))`+
		` (EXPRS (IDENT "m"))) `+
		`(BLOCK))`)
}

func TestStmtIfCond(t *testing.T) {
	p, _ := makeParser(`if cond { }`)
	compareNode(t, p.Parse(), `(if nil (IDENT "cond") (BLOCK) nil)`)
}

func TestStmtIfCondIsChannel(t *testing.T) {
	p, _ := makeParser(`if <-cond { }`)
	compareNode(t, p.Parse(), `(if nil (<- (IDENT "cond")) (BLOCK) nil)`)
}

func TestStmtIfInitCond(t *testing.T) {
	p, _ := makeParser(`if init; cond { b } else if c { d }`)
	compareNode(t, p.Parse(), `(if (IDENT "init") (IDENT "cond") (BLOCK (IDENT "b"))`+
		` (if nil (IDENT "c") (BLOCK (IDENT "d")) nil))`)
}

func TestStmtGo(t *testing.T) {
	p, _ := makeParser(`go f()`)
	compareNode(t, p.Parse(), `(go (CALL (IDENT "f")))`)
}

func TestStmtGoto(t *testing.T) {
	p, _ := makeParser(`loop: goto loop`)
	compareNode(t, p.Parse(), `(LABEL (IDENT "loop") (goto (IDENT "loop")))`)
}

func TestStmtSelect(t *testing.T) {
	p, _ := makeParser(`select { case a <- b: foo; case <- c: bar; case v,ok := <- a: baz; default: etc }`)
	compareNode(t, p.Parse(), `(select `+
		`(case (<- (IDENT "a") (IDENT "b")) (IDENT "foo")) `+
		`(case (<- (IDENT "c")) (IDENT "bar")) `+
		`(case (:= (EXPRS (IDENT "v") (IDENT "ok")) (EXPRS (<- (IDENT "a")))) (IDENT "baz")) `+
		`(default (IDENT "etc")))`)
}

func TestStmtSend(t *testing.T) {
	p, _ := makeParser(`channel <- v`)
	compareNode(t, p.Parse(), `(<- (IDENT "channel") (IDENT "v"))`)
}

func TestStmtSwitch(t *testing.T) {
	p, _ := makeParser(`switch { }`)
	compareNode(t, p.Parse(), `(switch nil nil)`)
}

func TestStmtSwitchExpr(t *testing.T) {
	p, _ := makeParser(`switch f() { }`)
	compareNode(t, p.Parse(), `(switch nil (CALL (IDENT "f")))`)
}
func TestStmtSwitchInit(t *testing.T) {
	p, _ := makeParser(`switch f() ; { }`)
	compareNode(t, p.Parse(), `(switch (CALL (IDENT "f")) nil)`)
}

func TestStmtSwitchInitExpr(t *testing.T) {
	p, _ := makeParser(`switch f; g { }`)
	compareNode(t, p.Parse(), `(switch (IDENT "f") (IDENT "g"))`)
}

func TestStmtSwitchInitExpr2(t *testing.T) {
	p, _ := makeParser(`switch a := init; b { case c, d: e; default: f }`)
	compareNode(t, p.Parse(), `(switch (:= (EXPRS (IDENT "a")) (EXPRS (IDENT "init"))) (IDENT "b") `+
		`(case (EXPRS (IDENT "c") (IDENT "d")) (IDENT "e")) `+
		`(default (IDENT "f")))`)
}

func TestStmtSwitchType(t *testing.T) {
	p, _ := makeParser(`switch x := a.b.(type) {  }`)
	compareNode(t, p.Parse(), `(TYPESWITCH nil (:=`+
		` (EXPRS (IDENT "x"))`+
		` (EXPRS (TYPE_ASSERT (. (IDENT "a") (IDENT "b")) (type)))))`)
}

func TestTopImport(t *testing.T) {
	p, _ := makeParser(`import _ "fmt"`)
	compareNode(t, p.Parse(), `(import (IMPORT_SPEC (IDENT "_") (STRING "\"fmt\"")))`)
}

func TestTopPackage(t *testing.T) {
	p, _ := makeParser(`package main`)
	compareNode(t, p.Parse(), `(package (IDENT "main"))`)
}

func TestTricky1(t *testing.T) {
	p, _ := makeParser(`channel <- *arr[typ{}.field]`)
	compareNode(t, p.Parse(), `(<- (IDENT "channel") (* (INDEX (IDENT "arr") (. (COMPOSITE_LIT (IDENT "typ")) (IDENT "field")))))`)
}

func TestTricky2(t *testing.T) {
	// 'x {' does not start a composite literal
	p, _ := makeParser(`for ; ; channel <- x {	}`)
	compareNode(t, p.Parse(), `(for nil nil (<- (IDENT "channel") (IDENT "x")) (BLOCK))`)
}

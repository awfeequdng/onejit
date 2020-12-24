/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * ourput.go
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"errors"
	"fmt"
)

func Debugf(format string, args ...interface{}) {
	fmt.Printf("// debug: "+format+"\n", args...)
}

var errorPrefix = "jit error: "

func Errorf(format string, args ...interface{}) {
	panic(errors.New(errorPrefix + fmt.Sprintf(format, args...)))
}

var assertError = errors.New("jit error: assertion failed")

func Assert(flag bool) {
	if !flag {
		panic(assertError)
	}
}

func badOpKind(op Op, kind Kind) Kind {
	Errorf("invalid operation: %v %v", op, kind)
	return Void // unreachable
}

func badOpKind2(op Op, kind1 Kind, kind2 Kind) Kind {
	Errorf("invalid operation: %v %v %v", op, kind1, kind2)
	return Void // unreachable
}

// ============================== Formatter ====================================

func (c Const) Format(st fmt.State, x rune) {
	fmt.Fprint(st, c.Interface())
}

func (r Reg) Format(st fmt.State, x rune) {
	fmt.Fprintf(st, "reg%x%s", r.id, r.kind.asmSuffix())
}

func (m Mem) Format(st fmt.State, x rune) {
	fmt.Fprintf(st, "mem%s%v", m.kind.asmSuffix(), m.addr)
}

func (l Label) Format(st fmt.State, x rune) {
	fmt.Fprintf(st, "label%x", l.index)
}

func (e UnaryExpr) Format(st fmt.State, x rune) {
	fmt.Fprintf(st, "(%v %v)", e.op, e.x)
}

func (e BinaryExpr) Format(st fmt.State, x rune) {
	fmt.Fprintf(st, "(%v %v %v)", e.x, e.op, e.y)
}

var (
	comma  = []byte(", ")
	rparen = []byte(")")
)

func (e TupleExpr) Format(st fmt.State, x rune) {
	fmt.Fprintf(st, "(%v", e.op)
	for _, expr := range e.list {
		fmt.Fprintf(st, " %v", expr)
	}
	st.Write(rparen)
}

func (e CallExpr) Format(st fmt.State, x rune) {
	fmt.Fprintf(st, "(CALL %v ", e.Func())
	for i, nin := 0, e.NumIn(); i < nin; i++ {
		if i != 0 {
			st.Write(comma)
		}
		fmt.Fprint(st, e.In(i))
	}
	st.Write(rparen)
}

func (f *Func) Format(st fmt.State, x rune) {
	fmt.Fprintf(st, "FUNC %v (", f.Name())
	for i, narg := 0, f.NumArg(); i < narg; i++ {
		if i != 0 {
			st.Write(comma)
		}
		fmt.Fprint(st, f.Arg(i))
	}
	if nret := f.NumRet(); nret > 0 {
		st.Write([]byte(") -> ("))
		for i := 0; i < nret; i++ {
			if i != 0 {
				st.Write(comma)
			}
			fmt.Fprint(st, f.Signature().Out(i))
		}
	}
	st.Write([]byte(") {\n"))
	for _, stmt := range f.code {
		fmt.Fprintf(st, "    %v\n", stmt)
	}
	st.Write([]byte("}\n"))
}

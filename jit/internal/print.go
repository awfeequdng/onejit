/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
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

package internal

import (
	"fmt"
)

// ============================== Formatter ====================================

func (c Const) Format(state fmt.State, x rune) {
	fmt.Fprint(state, c.Interface())
}

func (id RegId) Format(state fmt.State, x rune) {
	fmt.Fprintf(state, "rid%x", int(id))
}

func readonlyPrefix(ro bool) string {
	var s string
	if ro {
		s = "RO"
	}
	return s
}

func (r Reg) Format(state fmt.State, x rune) {
	fmt.Fprintf(state, "reg%s%x%s", readonlyPrefix(r.ro), int(r.id), r.kind.SizeString())
}

func (m Mem) Format(state fmt.State, x rune) {
	fmt.Fprintf(state, "(mem%s%s %v)", readonlyPrefix(m.ro), m.kind.SizeString(), m.addr)
}

func (m Amd64Mem) Format(state fmt.State, x rune) {
	var separator string
	fmt.Fprintf(state, "(mem%s%s ", readonlyPrefix(!m.IsAssignable()), m.kind.SizeString())
	if m.offset != 0 {
		fmt.Fprintf(state, "%d", m.offset)
		separator = " + "
	}
	if m.base != NoRegId {
		fmt.Fprintf(state, "%s%d", separator, m.base)
		separator = " + "
	}
	if m.index != NoRegId {
		fmt.Fprintf(state, "%s%v * %d", separator, m.index, m.Scale())
	}
	state.Write(rparen)
}

func (l Label) Format(state fmt.State, x rune) {
	fmt.Fprintf(state, "label%x", l.index)
}

func (e UnaryExpr) Format(state fmt.State, x rune) {
	fmt.Fprintf(state, "(%v %v)", e.op, e.x)
}

func (e BinaryExpr) Format(state fmt.State, x rune) {
	fmt.Fprintf(state, "(%v %v %v)", e.x, e.op, e.y)
}

var (
	comma  = []byte(", ")
	rparen = []byte(")")
)

func (e TupleExpr) Format(state fmt.State, x rune) {
	fmt.Fprintf(state, "(%v", e.op)
	for _, expr := range e.list {
		fmt.Fprintf(state, " %v", expr)
	}
	state.Write(rparen)
}

func (e CallExpr) Format(state fmt.State, x rune) {
	if fun := e.Func(); fun != nil {
		fmt.Fprintf(state, "(CALL %v ", fun.Name())
	} else {
		fmt.Fprintf(state, "(CALL %v ", e.FuncExpr())
	}
	for i, n := 0, e.NumArg(); i < n; i++ {
		if i != 0 {
			state.Write(comma)
		}
		fmt.Fprint(state, e.Arg(i))
	}
	state.Write(rparen)
}

func (s *ExprStmt) Format(state fmt.State, x rune) {
	s.print(&printer{state, x, 0})
}

func (s *IfStmt) Format(state fmt.State, x rune) {
	s.print(&printer{state, x, 0})
}

func (s *BlockStmt) Format(state fmt.State, x rune) {
	s.print(&printer{state, x, 0})
}

func (s *BreakStmt) Format(state fmt.State, x rune) {
	s.print(&printer{state, x, 0})
}

func (s *ContinueStmt) Format(state fmt.State, x rune) {
	s.print(&printer{state, x, 0})
}

func (s *ForStmt) Format(state fmt.State, x rune) {
	s.print(&printer{state, x, 0})
}

func (c *Compiled) Format(state fmt.State, x rune) {
	c.print(&printer{state, x, 0})
}

func (ac *ArchCompiled) Format(state fmt.State, x rune) {
	((*Compiled)(ac)).Format(state, x)
}

func (f *Func) Format(state fmt.State, x rune) {
	f.print(&printer{state, x, 0})
}

// ============================== printer ======================================

type printer struct {
	state fmt.State
	x     rune
	depth int
}

type printable interface {
	print(*printer)
}

var spaces = []byte("\n                                                                                ")

const spacen = 80

func (p *printer) write(str string) *printer {
	p.state.Write([]byte(str))
	return p
}

func (p *printer) format(obj fmt.Formatter) *printer {
	obj.Format(p.state, p.x)
	return p
}

func (p *printer) print(obj printable) *printer {
	obj.print(p)
	return p
}

func (p *printer) enter() *printer {
	p.depth += 4
	return p
}

func (p *printer) leave() *printer {
	p.depth -= 4
	return p
}

func (p *printer) nl() *printer {
	start := 0
	n := p.depth + 4
	for n > 0 {
		chunk := min2(n, spacen)
		p.state.Write(spaces[start : n+1])
		n -= chunk
		start = 1
	}
	return p
}

func min2(a, b int) int {
	if a > b {
		a = b
	}
	return a
}

func (s *ExprStmt) print(p *printer) {
	p.format(s.expr)
}

func (s *IfStmt) print(p *printer) {
	p.write("(IF ").format(s.cond.(fmt.Formatter))
	p.enter().nl().write("THEN ").print(s.Then())
	if s.Else() != nil {
		p.nl().write("ELSE ").print(s.Else())
	}
	p.leave().nl().state.Write(rparen)
}

func (s *BlockStmt) print(p *printer) {
	p.write("(BLOCK").enter()
	for _, stmt := range s.list {
		p.nl().print(stmt)
	}
	p.leave().nl().state.Write(rparen)
}

func (s *BreakStmt) print(p *printer) {
	p.write("(BREAK)")
}

func (s *ContinueStmt) print(p *printer) {
	p.write("(CONTINUE)")
}

func (s *ForStmt) print(p *printer) {
	p.write("(FOR ")
	if s.Init() != nil {
		p.print(s.Init()).write(" ")
	} else {
		p.write("nil ")
	}
	if s.Cond() != nil {
		p.format(s.Cond()).write(" ")
	} else {
		p.write("true ")
	}
	if s.Post() != nil {
		p.print(s.Post()).write(" ")
	} else {
		p.write("nil ")
	}
	if s.Body() != nil {
		p.enter().nl().print(s.Body()).leave()
	}
	p.nl().state.Write(rparen)
}

func (s *Source) print(p *printer) {
	for _, stmt := range s.list {
		p.nl().print(stmt)
	}
}

func (c *Compiled) print(p *printer) {
	c.fun.printHeader(p)
	for _, expr := range c.code {
		p.nl().format(expr)
	}
	c.fun.printFooter(p)
}

func (ac *ArchCompiled) print(p *printer) {
	((*Compiled)(ac)).print(p)
}

func (f *Func) print(p *printer) {
	f.printHeader(p)
	f.printSource(p)
	f.printFooter(p)
}

func (f *Func) printHeader(p *printer) {
	fmt.Fprintf(p.state, "FUNC %v (", f.Name())
	for i, narg := 0, f.NumArg(); i < narg; i++ {
		if i != 0 {
			p.state.Write(comma)
		}
		fmt.Fprint(p.state, f.Arg(i))
	}
	if nret := f.NumRet(); nret > 0 {
		p.state.Write([]byte(") -> ("))
		for i := 0; i < nret; i++ {
			if i != 0 {
				p.state.Write(comma)
			}
			fmt.Fprint(p.state, f.Signature().Out(i))
		}
	}
	p.write(") {").enter()
}

func (f *Func) printSource(p *printer) {
	p.print(&f.source)
}

func (f *Func) printFooter(p *printer) {
	p.leave().nl().write("}\n")
}

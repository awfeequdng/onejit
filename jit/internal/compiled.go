/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * compiled.go
 *
 *  Created on Dec 27, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

// ================================== Compiled =================================

type Compiled struct {
	fun    *Func
	regn   int
	labeln int
	code   []Expr
}

func (c *Compiled) Func() *Func {
	return c.fun
}

func (c *Compiled) Add(e Expr) *Compiled {
	if !e.IsPure() {
		c.code = append(c.code, e)
	}
	return c
}

func (c *Compiled) ToArch(archid ArchId) *ArchCompiled {
	ac := ArchCompiled{fun: c.fun}
	compile := Archs[archid].Compile
	if compile == nil {
		Warnf("unimplemented %v.Compile", archid)
	} else {
		for _, e := range c.code {
			e := compile(e, true, &ac)
			if e != nil {
				ac.Add(e)
			}
		}
	}
	return &ac
}

func (c *Compiled) Children() int {
	return len(c.code)
}

func (c *Compiled) Child(i int) Node {
	return c.code[i]
}
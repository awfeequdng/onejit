/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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

func (c *Compiled) ToArch(archid ArchId) *Asm {
	ac := Asm{fun: c.fun}
	arch := Archs[archid]
	compile := arch.Compile
	if compile == nil {
		Warnf("unimplemented %v.Compile", archid)
	} else if GetCpuWidth() != arch.CpuWidth {
		Errorf("GetCpuWidth() is currently %v, but Compiled.ToArch(%v) needs %v. Please call SetCpuWidth() *BEFORE* any other jit function",
			GetCpuWidth(), archid, arch.CpuWidth)
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

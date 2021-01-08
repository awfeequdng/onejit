/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
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
 * softreg.go
 *
 *  Created on Feb 10, 2019
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/onejit/go/jit_old/common"
)

func (c *Comp) NewSoftReg(kind Kind) SoftReg {
	id := c.nextSoftReg
	c.nextSoftReg++
	s := MakeSoftReg(id, kind)
	c.code.SoftReg(common.ALLOC, s)
	return s
}

func (c *Comp) newTempReg(kind Kind) SoftReg {
	id := c.nextTempReg
	c.nextTempReg++
	s := MakeSoftReg(id, kind)
	c.code.SoftReg(common.ALLOC, s)
	return s
}

func (c *Comp) FreeSoftReg(s SoftReg) {
	if s.Valid() && !s.IsTemp() {
		if s.Id()+1 == c.nextSoftReg {
			c.nextSoftReg--
		}
		c.code.SoftReg(common.FREE, s)
	}
}

func (c *Comp) freeTempReg(s SoftReg) {
	if s.Valid() && s.IsTemp() {
		if s.Id()+1 == c.nextTempReg {
			c.nextTempReg--
		}
		c.code.SoftReg(common.FREE, s)
	}
}

// alloc or free soft reg
func (c *Comp) SoftReg(inst Inst1Misc, s SoftReg) {
	if s.Valid() {
		c.code.SoftReg(inst.Asm(), s)
	}
}

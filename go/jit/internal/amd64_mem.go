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
 * amd64_mem.go
 *
 *  Created on Dec 26, 2020
 *      Author Massimiliano Ghilardi
 */

package internal

// amd64 memory location.
type Amd64Mem struct {
	offset  int32
	base    RegId
	index   RegId
	roscale uint8 // 1,2,4 or 8. add 128 if readonly
	kind    Kind
}

func MakeAmd64Mem(kind Kind, offset int32, base RegId, index RegId, scale uint8) Amd64Mem {
	if index != NoRegId {
		switch scale {
		case 1, 2, 4, 8:
			break
		default:
			Errorf("bad Amd64Mem scale: have %d, want 1,2,4 or 8", scale)
		}
	}
	return Amd64Mem{
		offset:  offset,
		base:    base,
		index:   index,
		roscale: scale,
		kind:    kind,
	}
}

func (m Amd64Mem) IsAssignable() bool {
	return m.roscale&128 == 0
}

// return a read-only view of a subset of memory contents
// note: memory does not become immutable - it can still be modified
// through the original struct
func (m Amd64Mem) ReadOnly(subset Kind) Amd64Mem {
	KindMustBeSubset("Amd64Mem", subset, m.kind)
	m.kind = subset
	m.roscale |= 128
	return m
}

func (m Amd64Mem) Offset() int32 {
	return m.offset
}

func (m Amd64Mem) BaseId() RegId {
	return m.base
}

func (m Amd64Mem) IndexId() RegId {
	return m.index
}

func (m Amd64Mem) Scale() uint8 {
	return m.roscale & 127
}

// implement Expr interface
func (m Amd64Mem) Class() Class {
	return MEM
}

func (m Amd64Mem) Op() Op {
	return VAR
}

func (m Amd64Mem) Kind() Kind {
	return m.kind
}

func (m Amd64Mem) expr() {}

func (m Amd64Mem) RegId() RegId {
	return NoRegId
}

func (m Amd64Mem) Size() Size {
	return m.kind.Size()
}

func (m Amd64Mem) Children() int {
	return 4
}

func (m Amd64Mem) Child(i int) Node {
	switch i {
	case 0:
		return ConstInt32(m.offset)
	case 1:
		if m.base == NoRegId {
			return nil
		}
		return archReg(Ptr, m.base)
	case 2:
		if m.index == NoRegId {
			return nil
		}
		return archReg(Int64, m.index)
	case 3:
		if m.index == NoRegId {
			return nil
		}
		return ConstUint8(m.Scale())
	}
	return BadIndex(i, 4)
}

func (m Amd64Mem) IsConst() bool {
	// memory access cannot be a compile-time constant
	return false
}

func (m Amd64Mem) IsPure() bool {
	// reading from memory has no side effects
	return true
}

/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * const.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package internal

import (
	"math"
	"unsafe"
)

type Const struct {
	kind Kind
	re   int64
	im   int64
}

var (
	False = MakeConst(Bool, 0)
	True  = MakeConst(Bool, 1)
)

// implement Expr interface
func (c Const) Class() Class {
	return CONSTANT
}

func (c Const) Op() Op {
	return CONST
}

func (c Const) Kind() Kind {
	return c.kind
}

func (c Const) expr() {}

func (c Const) RegId() RegId {
	return NoRegId
}

func (c Const) Size() Size {
	return c.kind.Size()
}

func (c Const) Children() int {
	return 0
}

func (c Const) Child(i int) Node {
	return BadIndex(i, 0)
}

func (c Const) IsConst() bool {
	return true
}
func (c Const) IsPure() bool {
	return true
}

// ============================= helpers =======================================

// save Const to a temporary register
func (c Const) spillToReg(ac *ArchCompiled) Reg {
	reg := ac.Func().NewReg(c.kind)
	ac.Add(Binary(ASSIGN, reg, c))
	return reg
}

var (
	ifalse interface{} = false
	itrue  interface{} = true
)

func (c Const) IsZero() bool {
	return c.re == 0 && c.im == 0
}

func (c Const) Int() int64 {
	return c.re
}

func (c Const) Uint() uint64 {
	return uint64(c.re)
}

func (c Const) Float32() float32 {
	return math.Float32frombits(uint32(c.re))
}

func (c Const) Float64() float64 {
	return math.Float64frombits(uint64(c.re))
}

func (c Const) Complex64() complex64 {
	return complex(
		math.Float32frombits(uint32(c.re)),
		math.Float32frombits(uint32(c.im)))
}

func (c Const) Complex128() complex128 {
	return complex(
		math.Float64frombits(uint64(c.re)),
		math.Float64frombits(uint64(c.im)))
}

func (c Const) Ptr() unsafe.Pointer {
	return unsafe.Pointer(uintptr(c.re))
}

func (c Const) Interface() interface{} {
	var i interface{}
	val := c.re
	targetIs32bit := cpuWidth == 32
	switch c.kind {
	case Bool:
		if val == 0 {
			i = ifalse
		} else {
			i = itrue
		}
	case Int:
		if targetIs32bit {
			i = int32(val)
		}
	case Int8:
		i = int8(val)
	case Int16:
		i = int16(val)
	case Int32:
		i = int32(val)
	case Int64:
		i = int64(val)
	case Uint, Uintptr:
		if targetIs32bit {
			i = uint32(val)
		} else {
			i = uint64(val)
		}
	case Uint8:
		i = uint8(val)
	case Uint16:
		i = uint16(val)
	case Uint32:
		i = uint32(val)
	case Uint64:
		i = uint64(val)
	case Float32:
		i = c.Float32()
	case Float64:
		i = c.Float64()
	case Complex64:
		i = c.Complex64()
	case Complex128:
		i = c.Complex128()
	case Ptr:
		i = c.Ptr()
	}
	return i
}

// convert Const to a different kind
func (c Const) Cast(to Kind) Const {
	val := c.re
	// sign-extend or zero-extend to 64 bits
	switch c.kind {
	case Bool:
		if val != 0 {
			// non-zero means true => convert to 1
			val = 1
		}
	case Int:
		val = int64(int(val))
	case Int8:
		val = int64(int8(val))
	case Int16:
		val = int64(int16(val))
	case Int32:
		val = int64(int32(val))
	case Int64:
		// nothing to do
	case Uint:
		val = int64(uint(val))
	case Uint8:
		val = int64(uint8(val))
	case Uint16:
		val = int64(uint16(val))
	case Uint32:
		val = int64(uint32(val))
	case Uint64:
		val = int64(uint64(val)) // should be a nop
	case Uintptr:
		val = int64(uint64(val))
	default:
		Errorf("Const.Cast: unsupported constant kind: %v", c.kind)
	}
	// let caller truncate val as needed
	return Const{kind: to, re: val}
}

func MakeConst(kind Kind, val int64) Const {
	return Const{kind: kind, re: val}
}

func MakeConst2(kind Kind, re int64, im int64) Const {
	return Const{kind: kind, re: re, im: im}
}

func ConstBool(flag bool) Const {
	c := Const{kind: Bool}
	if flag {
		c.re = 1
	}
	return c
}

// target 'int' may be 64 bits
func ConstInt(val int64) Const {
	return Const{kind: Int, re: val}
}

func ConstInt8(val int8) Const {
	return Const{kind: Int8, re: int64(val)}
}

func ConstInt16(val int16) Const {
	return Const{kind: Int16, re: int64(val)}
}

func ConstInt32(val int32) Const {
	return Const{kind: Int32, re: int64(val)}
}

func ConstInt64(val int64) Const {
	return Const{kind: Int64, re: val}
}

// target 'uint' may be 64 bits
func ConstUint(val uint64) Const {
	return Const{kind: Uint, re: int64(val)}
}

func ConstUint8(val uint8) Const {
	return Const{kind: Uint8, re: int64(val)}
}

func ConstUint16(val uint16) Const {
	return Const{kind: Uint16, re: int64(val)}
}

func ConstUint32(val uint32) Const {
	return Const{kind: Uint32, re: int64(val)}
}

func ConstUint64(val uint64) Const {
	return Const{kind: Uint64, re: int64(val)}
}

// target 'uintptr' may be 64 bits
func ConstUintptr(val uint64) Const {
	return Const{kind: Uintptr, re: int64(val)}
}

func ConstFloat32(val float32) Const {
	return Const{kind: Float32, re: int64(math.Float32bits(val))}
}

func ConstFloat64(val float64) Const {
	return Const{kind: Float64, re: int64(math.Float64bits(val))}
}

func ConstComplex64(val complex64) Const {
	return Const{
		kind: Complex64,
		re:   int64(math.Float32bits(real(val))),
		im:   int64(math.Float32bits(imag(val))),
	}
}

func ConstComplex128(val complex128) Const {
	return Const{
		kind: Complex128,
		re:   int64(math.Float64bits(real(val))),
		im:   int64(math.Float64bits(imag(val))),
	}
}

func ConstZero(kind Kind) Const {
	return Const{kind: kind}
}

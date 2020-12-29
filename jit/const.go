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
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/onejit/jit/internal"
)

var (
	False = internal.False
	True  = internal.True
)

// ============================= helpers =======================================

func MakeConst(kind Kind, val int64) Const {
	return internal.MakeConst(kind, val)
}

func MakeConst2(kind Kind, re int64, im int64) Const {
	return internal.MakeConst2(kind, re, im)
}

func ConstBool(flag bool) Const {
	return internal.ConstBool(flag)
}

// target 'int' may be 64 bits
func ConstInt(val int64) Const {
	return internal.ConstInt(val)
}

func ConstInt8(val int8) Const {
	return internal.ConstInt8(val)
}

func ConstInt16(val int16) Const {
	return internal.ConstInt16(val)
}

func ConstInt32(val int32) Const {
	return internal.ConstInt32(val)
}

func ConstInt64(val int64) Const {
	return internal.ConstInt64(val)
}

// target 'uint' may be 64 bits
func ConstUint(val uint64) Const {
	return internal.ConstUint(val)
}

func ConstUint8(val uint8) Const {
	return internal.ConstUint8(val)
}

func ConstUint16(val uint16) Const {
	return internal.ConstUint16(val)
}

func ConstUint32(val uint32) Const {
	return internal.ConstUint32(val)
}

func ConstUint64(val uint64) Const {
	return internal.ConstUint64(val)
}

// target 'uintptr' may be 64 bits
func ConstUintptr(val uint64) Const {
	return internal.ConstUintptr(val)
}

func ConstZero(kind Kind) Const {
	return internal.ConstZero(kind)
}

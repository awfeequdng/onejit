/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * type.go
 *
 *  Created on: Mar 31, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type Type interface {
	String() string
	Underlying() Type
	common() *Complete
}

type (
	flags   uint32
	ChanDir flags
)

const (
	RecvDir ChanDir = 1
	SendDir ChanDir = 2
	BothDir         = RecvDir | SendDir

	isVariadic flags = 4
	isComplete flags = 8

	unknownSize = ^uint64(0)
)

// configurable, must be 4 or 8
var sizeOfInt uint64 = detectSizeOfInt()

func detectSizeOfInt() uint64 {
	if ^uint(0)>>31 == 1 {
		return 4
	}
	return 8
}

// return current sizeOfInt - either 4 or 8
func SizeOfInt() uint64 {
	return sizeOfInt
}

// Set sizeOfInt to either 4 or 8.
// Specifying zero means autodetect from compile architecture.
// Also causes BasicType() and BasicTypes() to return different results
func SetSizeOfInt(size uint64) {
	if size == 0 {
		size = detectSizeOfInt()
	}
	if size != 4 && size != 8 {
		panic("SetSizeOfInt: unsupported size, expecting 4 or 8")
	}
	sizeOfInt = size
}

// return bitwise AND of specified type's flags
func flagsAnd(list []Type) flags {
	ret := ^flags(0)
	for _, t := range list {
		ret &= t.common().flags
	}
	return ret
}

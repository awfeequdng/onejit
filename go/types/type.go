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

import (
	"strings"
)

type Type interface {
	String() string
	Underlying() Type
	common() *Complete
	complete() // completes everything in the type, except flags & flagComplete
	writeTo(*strings.Builder, verbose)
}

func TypeString(t Type, showFullPkgPath bool) string {
	if showFullPkgPath {
		return t.String()
	}
	return t.common().str
}

type (
	ArchSizeBits uint64

	flags   uint32
	ChanDir flags
)

const (
	ArchSize32 ArchSizeBits = 32
	ArchSize64 ArchSizeBits = 64
	// autodetect ArchSizeBits from compile architecture
	ArchSizeAuto = 32 * (1 + ArchSizeBits(^uint(0)>>63))

	RecvDir ChanDir = 1
	SendDir ChanDir = 2
	BothDir         = RecvDir | SendDir

	flagNillable      flags = 4
	flagComparable    flags = 8
	flagNotComparable flags = 16
	flagComplete      flags = 32
	flagNeedPadding   flags = 64 // type is or ends with zero-byte struct or array
	flagVariadic      flags = 128

	unknownSize = ^uint64(0)
)

// configurable, must be 4 or 8
var archSizeBits ArchSizeBits = ArchSizeAuto
var archSizeBytes uint64 = uint64(ArchSizeAuto) / 8

// return current archSizeBits - either 32 or 64
func GetArchSizeBits() ArchSizeBits {
	return archSizeBits
}

// Set archSizeBits to either 32 or 64.
// To autodetect from compile architecture, specify ArchSizeAuto.
// Also causes BasicType() and BasicTypes() to return different results
func SetArchSizeBits(size ArchSizeBits) {
	if size != ArchSize32 && size != ArchSize64 {
		panic("SetArchSizeBits: unsupported size, expecting 32 or 64")
	}
	archSizeBits = size
	archSizeBytes = uint64(size) / 8
}

// return bitwise AND of specified type's flags
func flagsAnd(list []Type) flags {
	ret := ^flags(0)
	for _, t := range list {
		ret &= t.common().flags
	}
	return ret
}

// return bitwise AND of specified method type's flags
func flagsAndMethod(list []Method) flags {
	ret := ^flags(0)
	for i := range list {
		ret &= list[i].Type.common().flags
	}
	return ret
}

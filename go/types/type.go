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
	flags   uint16
	ChanDir flags
)

const (
	RecvDir ChanDir = 1
	SendDir ChanDir = 2
	BothDir         = RecvDir | SendDir

	flagComparable    flags = 4
	flagNotComparable flags = 8
	flagComplete      flags = 16
	flagNeedPadding   flags = 32 // type is or ends with zero-byte struct or array
	flagVariadic      flags = 64

	unknownSize  = ^uint64(0)
	unknownAlign = ^uint16(0)
)

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

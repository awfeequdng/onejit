/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * string.go
 *
 *  Created on: Apr 02, 2021
 *      Author: Massimiliano Ghilardi
 */

package strings

import "strings"

type Builder = strings.Builder

type Verbose bool

const (
	ShortPkgName Verbose = false
	FullPkgPath  Verbose = true
)

func Basename(path string) string {
	slash := strings.LastIndexByte(path, '/')
	return path[slash+1:]
}

func UintToString(n uint64) string {
	if n == 0 {
		return "0"
	}
	var b [20]byte
	pos := len(b)
	for n != 0 {
		pos--
		b[pos] = '0' + byte(n%10)
		n /= 10
	}
	return string(b[pos:])
}

func WriteQualifiedName(b *strings.Builder, name string, pkgPath string, flag Verbose) {
	if flag == ShortPkgName {
		pkgPath = Basename(pkgPath)
	}
	if len(pkgPath) != 0 {
		b.WriteString(pkgPath)
		b.WriteByte('.')
	}
	b.WriteString(name)
}

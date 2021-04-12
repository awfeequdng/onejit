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

import "github.com/cosmos72/onejit/go/io"

type Verbose bool

const (
	ShortPkgName Verbose = false
	FullPkgPath  Verbose = true
)

func Basename(path string) string {
	slash := LastIndexByte(path, '/')
	return path[slash+1:]
}

// return the offset of the first byte == b in str, or -1 if str does not contain b
func IndexByte(str string, b byte) int {
	n := len(str)
	for i := 0; i < n; i++ {
		if str[i] == b {
			return i
		}
	}
	return -1
}

// return the offset of the last byte == b in str, or -1 if str does not contain b
func LastIndexByte(str string, b byte) int {
	n := len(str)
	i := n - 1
	for ; i >= 0; i-- {
		if str[i] == b {
			break
		}
	}
	return i
}

func WriteQualifiedName(out io.StringWriter, name string, pkgPath string, flag Verbose) {
	if flag == ShortPkgName {
		pkgPath = Basename(pkgPath)
	}
	if len(pkgPath) != 0 {
		out.WriteString(pkgPath)
		out.WriteString(".")
	}
	out.WriteString(name)
}

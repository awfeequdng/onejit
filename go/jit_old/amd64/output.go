/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * output.go
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

package amd64

import (
	"errors"
	"fmt"
)

func debugf(format string, args ...interface{}) {
	fmt.Printf("// debug amd64: "+format+"\n", args...)
}

var errorPrefix = "amd64 assembler error: "

func errorf(format string, args ...interface{}) {
	panic(errors.New(errorPrefix + fmt.Sprintf(format, args...)))
}

var assertError = errors.New("amd64 assembler error, assertion failed")

func assert(flag bool) {
	if !flag {
		panic(assertError)
	}
}

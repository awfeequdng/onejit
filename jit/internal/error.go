/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * ourput.go
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

package internal

import (
	"errors"
	"fmt"
)

func Debugf(format string, args ...interface{}) {
	fmt.Printf("// debug: "+format+"\n", args...)
}

func Warnf(format string, args ...interface{}) {
	fmt.Printf("// warning: "+format+"\n", args...)
}

var errorPrefix = "jit error: "

func Errorf(format string, args ...interface{}) {
	panic(errors.New(errorPrefix + fmt.Sprintf(format, args...)))
}

var assertError = errors.New("jit error: assertion failed")

func Assert(flag bool) {
	if !flag {
		panic(assertError)
	}
}

func BadOpKind(op Op, kind Kind) Kind {
	Errorf("invalid operation: %v %v", op, kind)
	return Void // unreachable
}

func BadOpKind2(op Op, kind1 Kind, kind2 Kind) Kind {
	Errorf("invalid operation: %v %v %v", op, kind1, kind2)
	return Void // unreachable
}

func BadIndex(i int, n int) Node {
	Errorf("index out of range: %v with length %v", i, n)
	return nil // unreachable
}

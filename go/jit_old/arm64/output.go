/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
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
 * output.go
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

package arm64

import (
	"errors"
	"fmt"
)

func debugf(format string, args ...interface{}) {
	fmt.Printf("// debug arm64: "+format+"\n", args...)
}

var errorPrefix = "arm64 assembler error: "

func errorf(format string, args ...interface{}) {
	panic(errors.New(errorPrefix + fmt.Sprintf(format, args...)))
}

var assertError = errors.New("arm64 assembler internal error, assertion failed")

func assert(flag bool) {
	if !flag {
		panic(assertError)
	}
}

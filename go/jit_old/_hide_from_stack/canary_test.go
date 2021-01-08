/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
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
 * canary_test.go
 *
 *  Created on Oct 27, 2019
 *      Author Massimiliano Ghilardi
 */

package hide_from_stack

import (
	"fmt"
	"runtime"
	"strings"
)

func canary(arg uintptr) {
	fmt.Printf("canary(%d) called", arg)
	pcs := make([]uintptr, 3)
	n := runtime.Callers(2, pcs)
	if n > 0 {
		frames := runtime.CallersFrames(pcs)
		for _, pc := range pcs {
			frame, more := frames.Next()
			if frame.PC != 0 {
				function := frame.Function[1+strings.LastIndexByte(frame.Function, '.'):]
				fmt.Printf(" by %s()%s", function, "               "[(len(function)+15)&15:])
			} else {
				fmt.Printf(" by 0x%08x\t", pc)
			}
			if !more {
				break
			}
		}
	}
	fmt.Println()
	runtime.GC()
}

// return a closure
func make_parrot(arg0 uintptr) func(uintptr) {
	return func(arg1 uintptr) {
		fmt.Printf("parrot(%d) called, closure data = %d\n", arg1, arg0)
		// debug.PrintStack()
		runtime.GC()
	}
}

func address_of_canary() func(uintptr) {
	return canary
}

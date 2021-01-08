// +build !gc !amd64,!arm64

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
 * api_stub.go
 *
 *  Created on Oct 27, 2019
 *      Author Massimiliano Ghilardi
 */

package hide_from_stack

// Return an array of function addresses {call0, call16 ... call512}
// If unsupported on this CPU or compiler, all values will be zero.
func MakeCallArray() [7]uintptr {
	return [7]uintptr{}
}

func call0() {
	unimplemented()
}
func call16() {
	unimplemented()
}
func call32() {
	unimplemented()
}
func call64() {
	unimplemented()
}
func call128() {
	unimplemented()
}
func call256() {
	unimplemented()
}

// ensure stack has > 512 free bytes
func GrowStack() {
	unimplemented()
}

func hidden_jit_func(uintptr) {
	unimplemented()
}

// go:noinline
func unimplemented() {
	panic("hide_from_stack: not implemented on this CPU and/or compiler")
}

// +build gc,amd64 gc,arm64

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
 * api.go
 *
 *  Created on Oct 27, 2019
 *      Author Massimiliano Ghilardi
 */

package hide_from_stack

// Return an array of function addresses {call0, call16 ... call512}
// If unsupported on this CPU or compiler, all values will be zero.
func MakeCallArray() [7]uintptr {
	var ret [7]uintptr
	for i, call := range [...]func(){
		call0, call16, call32, call64, call128, call256, call512,
	} {
		ret[i] = deconstruct_func0(call).funcAddress
	}
	return ret
}

/**
 * call the closure stored in DX (which expects exactly 0 bytes of arguments
 * + return values), hiding the caller from runtime stack:
 * on AMD64, caller is replaced with a fake entry hidden_jit_func()
 * on ARM64, caller completely disappears from stack trace
 */
func call0()

/**
 * call the closure stored in DX, hiding the caller from runtime stack:
 * on AMD64, caller is replaced with a fake entry hidden_jit_func()
 * on ARM64, caller completely disappears from stack trace

 * the closure can expect up to 16 bytes of arguments + return values
 *
 * writing arguments in call16() stack and retrieving return values from it
 * are caller's responsibility
 * (possible only in assembly: caller has to access call16() stack)
 */
func call16()

// as above, but closure can expect up to 32 bytes of arguments return values
func call32()

// as above, but closure can expect up to 64 bytes of arguments return values
func call64()

// as above, but closure can expect up to 128 bytes of arguments return values
func call128()

// as above, but closure can expect up to 256 bytes of arguments return values
func call256()

// as above, but closure can expect up to 512 bytes of arguments return values
func call512()

// ensure stack has > 512 free bytes
func GrowStack()

// on AMD64, hidden JIT functions will be replaced by this function in the stack trace
func hidden_jit_func(uintptr)

// Reference implementation of JIT function, showing how write
// a function without stack map, frame, local variables on stack;
// yet it can invoke arbitrary Go functions through the env.call[] trampolines
func asm_hideme(env *Env)

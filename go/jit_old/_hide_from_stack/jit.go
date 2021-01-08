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
 * jit.go
 *
 *  Created on Oct 31, 2019
 *      Author Massimiliano Ghilardi
 */

package hide_from_stack

import (
	"unsafe"

	"github.com/cosmos72/onejit/go/jit_old/asm"
)

type Env struct {
	closure func(uintptr)
	arg     uintptr
	call    [7]uintptr // call0, call16 ... call512
}

var jit_hideme func(*Env) = make_jit(asm_hideme)

var jit_fails func(*Env) = make_jit(call_closure)

// if the same assembly as this function is executed from runtime-allocated memory,
// it does not have a stackmap thus calling (almost all) Go functions crashes the garbage collector
func call_closure(env *Env) {
	env.closure(env.arg)
}

// return a copy of fun allocated in executable mmap'ed memory,
// as a JIT compiler would do.
// fun must occupy 128 bytes or less.
func make_jit(fun func(*Env)) func(*Env) {
	if !asm.ARCH_SUPPORTED {
		return nil
	}
	var ret func(*Env)
	addr := (*[128]byte)(unsafe.Pointer(deconstruct_any_func(fun).funcAddress))[:]
	asm.New().Bytes(addr...).Func(&ret)
	return ret
}

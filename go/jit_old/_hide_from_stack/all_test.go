// +build gc,amd64 gc,arm64

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
 * all_test.go
 *
 *  Created on Oct 27, 2019
 *      Author Massimiliano Ghilardi
 */

package hide_from_stack

import (
	"fmt"
	"testing"
)

func asm_address_of_canary() func(uintptr)
func asm_call_canary(arg uintptr)
func asm_call_func(func_address uintptr, arg uintptr)
func asm_call_closure(tocall func(uintptr), arg uintptr)
func asm_loop()

/*
func TestAsmLoop(t *testing.T) {
	go asm_loop() // causes runtime.GC() to wait indefinitely
	runtime.GC()
}
*/

func TestAddressOfCanary(t *testing.T) {
	if false {
		fmt.Printf("canary                       = %p\n", canary)
		fmt.Printf("address_of_canary()          = %p\n", address_of_canary())
		fmt.Printf("asm_address_of_canary()      = %p\n", asm_address_of_canary())
		header := deconstruct_func8(canary)
		fmt.Printf("deconstruct_func8(canary)    = %#x\n", *header)
		header = deconstruct_any_func(canary)
		fmt.Printf("deconstruct_any_func(canary) = %#x\n", *header)
	}
}

func TestCallCanary(t *testing.T) {
	asm_call_canary(0)
	asm_call_func(deconstruct_any_func(canary).funcAddress, 1)
	asm_call_closure(asm_address_of_canary(), 2)
	GrowStack()
	env := &Env{canary, 3, MakeCallArray()}
	asm_hideme(env)
	if jit_hideme != nil {
		env.arg++
		jit_hideme(env)
	}
}

func TestCallParrot(t *testing.T) {
	parrot := make_parrot(123456)
	asm_call_closure(parrot, 0)
	env := &Env{parrot, 1, MakeCallArray()}
	asm_hideme(env)
	if jit_hideme != nil {
		env.arg++
		jit_hideme(env)
	}
}

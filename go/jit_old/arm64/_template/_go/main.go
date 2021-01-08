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
 * main.go
 *
 *  Created on Feb 02, 2019
 *      Author Massimiliano Ghilardi
 */

package main

import (
	"fmt"
	"reflect"
)

type EnvBinds struct {
	Vals []reflect.Value
	Ints []uint64
}

// simplified fast.Env
type Env struct {
	EnvBinds
	Outer   *Env
	IP      int
	Code    []Stmt
	Run     *Run
	FileEnv *Env
}

type Run struct {
}

type Stmt func(*Env) (Stmt, *Env)

func main() {
	fmt.Println(Add8, Add16, Add32, Add64,
		Zero0, Zero8, Zero16, Zero32, Zero64,
		Div8, Div16, Div32, Div64,
		UDiv8, UDiv16, UDiv32, UDiv64,
		Nop, Leave)

	var env Env
	t := reflect.TypeOf(env)
	showFields(t, "Ints", "Outer", "FileEnv")
}

func showFields(t reflect.Type, names ...string) {
	for _, name := range names {
		f, _ := t.FieldByName(name)
		fmt.Printf("%#v\n", f)
	}
}

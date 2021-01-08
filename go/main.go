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
 *  Created on Nov 23, 2019
 *      Author Massimiliano Ghilardi
 */

package main

import (
	. "github.com/cosmos72/onejit/go/jit"
	_ "github.com/cosmos72/onejit/go/jit/amd64"
	_ "github.com/cosmos72/onejit/go/jit/arm64"
	_ "github.com/cosmos72/onejit/go/jit/x86"
	_ "github.com/cosmos72/onejit/go/jit_old"
)

func main() {
	f := NewFunc("main", NewSignature(nil, nil))
	f.Compile()
}

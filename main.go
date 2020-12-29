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
 * main.go
 *
 *  Created on Nov 23, 2019
 *      Author Massimiliano Ghilardi
 */

package main

import (
	. "github.com/cosmos72/onejit/jit"
	_ "github.com/cosmos72/onejit/jit/amd64"
	_ "github.com/cosmos72/onejit/jit/arm64"
	_ "github.com/cosmos72/onejit/jit/x86"
	_ "github.com/cosmos72/onejit/jit_old"
)

func main() {
	f := NewFunc("main", NewSignature(nil, nil))
	f.Compile()
}

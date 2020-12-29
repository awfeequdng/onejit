/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * arch.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/onejit/jit/internal"
)

const (
	NOARCH = internal.NOARCH
	AMD64  = internal.AMD64
	ARM64  = internal.ARM64
	ARM    = internal.ARM
	X86    = internal.X86
)

var Archs = internal.Archs

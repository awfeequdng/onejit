/*
 * gomacrojitjit - JIT compiler in Go
 *
 * Copyright (C) 2019 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * alias.go
 *
 *  Created on Feb 10, 2019
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/gomacrojit/jit/common"
)

type (
	ArchId = common.ArchId
	Size   = common.Size
)

const (
	// ArchId
	NOARCH = common.NOARCH
	AMD64  = common.AMD64
	ARM64  = common.ARM64
)

/*
 * gomacrojit - JIT compiler in Go
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
 *  Created on Feb 13, 2019
 *      Author Massimiliano Ghilardi
 */

package jit

type ArchId uint8

const (
	NOARCH ArchId = iota
	AMD64
	ARM64
	ARM
	X86
)

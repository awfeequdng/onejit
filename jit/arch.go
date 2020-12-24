/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2019-2020 Massimiliano Ghilardi
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

import (
	"fmt"
)

type ArchId uint8

const (
	NOARCH ArchId = iota
	AMD64
	ARM64
)

type Arch interface {
	Id() ArchId
	String() string
}

var Archs = make(map[ArchId]Arch) // {ARM64:Arm64{}, AMD64:Amd64{}}

func (archId ArchId) String() string {
	arch := Archs[archId]
	if arch != nil {
		return arch.String()
	}
	return fmt.Sprintf("ArchId(%d)", uint8(archId))
}

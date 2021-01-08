// +build !darwin,!dragonfly,!freebsd,!linux,!netbsd,!openbsd,!windows

/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
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
 * mmap_generic.go
 *
 *  Created on May 24, 2018
 *      Author Massimiliano Ghilardi
 */

package common

import (
	"runtime"
)

const MMAP_SUPPORTED = false

type MemPool struct {
}

type MemArea struct {
}

func (pool *MemPool) Size() int {
	return 0
}

func NewMemPool(size int) *MemPool {
	errorf("MemPool: unsupported operating system %v, cannot create executable memory", runtime.GOOS)
	return nil
}

func (pool *MemPool) SetReadonly() {
}

func (pool *MemPool) SetReadWrite() {
}

func (pool *MemPool) Copy(area MemArea) MemArea {
	errorf("MemPool: unsupported operating system %v, cannot copy machine code to executable memory", runtime.GOOS)
	return MemArea{}
}

// convert MachineCode to MemArea
func (code MachineCode) MemArea() MemArea {
	return MemArea{}
}

func (area MemArea) Addr() *uint8 {
	return nil
}

func (area MemArea) Size() int {
	return 0
}

func (area MemArea) Equal(other MemArea) bool {
	return false
}

func (area MemArea) Checksum() uint32 {
	return 0
}

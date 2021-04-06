/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * arch.go
 *
 *  Created on: Apr 06, 2021
 *      Author: Massimiliano Ghilardi
 */

package arch

type (
	Arch uint16
	Bits uint16
)

const (
	I386 Arch = iota
	Amd64
	Arm
	Arm64
	ArchOther32bit
	ArchOther64bit
	arch_end
)

const (
	Bits32 Bits = 32
	Bits64 Bits = 64
)

func (arch Arch) String() string {
	var str string
	switch arch {
	case I386:
		str = "386"
	case Amd64:
		str = "amd64"
	case Arm:
		str = "arm"
	case Arm64:
		str = "arm64"
	case ArchOther32bit:
		str = "other32bit"
	case ArchOther64bit:
		str = "other64bit"
	default:
		str = "unknown"
	}
	return str
}

// return sizeof(uintptr) in bits for specified architecture
func (arch Arch) Bits() Bits {
	switch arch {
	case I386, Arm, ArchOther32bit:
		return Bits32
	default:
		return Bits64
	}
}

// return sizeof(uintptr) in bytes for specified architecture
func (arch Arch) Bytes() uint64 {
	return uint64(arch.Bits() / 8)
}

// return max addressable memory size for specified architecture
func (arch Arch) MaxSizeBytes() uint64 {
	return ^(^uint64(0) << arch.Bits())
}

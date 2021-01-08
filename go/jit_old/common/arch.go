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
 * arch.go
 *
 *  Created on Feb 13, 2019
 *      Author Massimiliano Ghilardi
 */

package common

import (
	"fmt"
)

type ArchId uint8

const (
	NOARCH ArchId = iota
	ARM64
	AMD64
)

type Arch interface {
	Id() ArchId
	String() string
	RegIdConfig() RegIdConfig
	RegIdValid(id RegId) bool
	RegIdKind(id RegId) Kind     // uint64 or float64
	RegIdString(id RegId) string // RegId -> string
	RegValid(r Reg) bool
	RegString(r Reg) string          // Reg -> string
	MemString(m Mem) string          // Mem -> string
	CodeString(c MachineCode) string // Code -> string

	Init(asm *Asm, saveStart, saveEnd SaveSlot) *Asm
	Prologue(asm *Asm) *Asm
	Epilogue(asm *Asm) *Asm

	Op0(asm *Asm, op Op0) *Asm
	Op1(asm *Asm, op Op1, dst Arg) *Asm
	Op2(asm *Asm, op Op2, src Arg, dst Arg) *Asm
	Op3(asm *Asm, op Op3, a Arg, b Arg, dst Arg) *Asm
	Op4(asm *Asm, op Op4, a Arg, b Arg, c Arg, dst Arg) *Asm
}

var Archs = make(map[ArchId]Arch) // {ARM64:Arm64{}, AMD64:Amd64{}}

func (archId ArchId) String() string {
	arch := Archs[archId]
	if arch != nil {
		return arch.String()
	}
	return fmt.Sprintf("ArchId(%d)", uint8(archId))
}

func (code MachineCode) String() string {
	arch := Archs[code.ArchId]
	if arch != nil {
		return arch.CodeString(code)
	}
	return fmt.Sprintf("%x", code.Bytes)
}

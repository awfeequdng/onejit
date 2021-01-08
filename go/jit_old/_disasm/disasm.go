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
 * api.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package disasm

import (
	"testing"
	"unsafe"

	"github.com/bnagy/gapstone"
	. "github.com/cosmos72/onejit/go/jit_old"
	"github.com/cosmos72/onejit/go/jit_old/asm"
)

type Engine = gapstone.Engine

func NewDisasm(archId ArchId) (Engine, error) {
	var arch uint = gapstone.CS_ARCH_X86
	var mode uint = gapstone.CS_MODE_64
	if archId == ARM64 {
		arch = gapstone.CS_ARCH_ARM64
		mode = gapstone.CS_MODE_ARM // | gapstone.CS_MODE_V8
	}
	engine, err := gapstone.New(
		int(arch),
		mode,
	)
	if err != nil {
		return engine, err
	}
	engine.SetOption(gapstone.CS_OPT_SYNTAX, gapstone.CS_OPT_SYNTAX_ATT)
	return engine, nil
}

func Disasm(code asm.MachineCode) ([]gapstone.Instruction, error) {
	engine, err := NewDisasm(code.ArchId)
	if err != nil {
		return nil, err
	}
	address := uint64(0x10000)
	if len(code.Bytes) != 0 {
		address = uint64(uintptr(unsafe.Pointer(&code.Bytes[0])))
	}
	return engine.Disasm(code.Bytes, address, 0)
}

func PrintDisasm(t *testing.T, code asm.MachineCode) {
	insns, err := Disasm(code)
	if err != nil {
		t.Error(err)
	} else {
		archId := code.ArchId
		t.Logf("----- %v -----", archId)
		for _, insn := range insns {
			Show(t, archId, insn)
		}
	}
}

func Show(t *testing.T, archId ArchId, insn gapstone.Instruction) {
	var prefix string
	bytes := insn.Bytes
	if archId == ARM64 && len(bytes) == 4 {
		// print high byte first
		prefix = "0x"
		bytes[0], bytes[1], bytes[2], bytes[3] = bytes[3], bytes[2], bytes[1], bytes[0]
	}
	t.Logf(" %s%x%s%s\t%s", prefix, bytes, spaces(2*len(insn.Bytes)), insn.Mnemonic, insn.OpStr)
}

func spaces(n int) string {
	return "                "[n%16:]
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_number_test.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package scanner

import (
	"testing"

	"github.com/cosmos72/onejit/go/strings"
	"github.com/cosmos72/onejit/go/token"
)

func TestPosition(t *testing.T) {
	offset := 123000

	file := token.NewFile("repl.go", offset)
	var reader strings.Reader
	var s Scanner

	str := "oregh85hgdolgj\n&*(^\n90"
	reader.Reset(str)
	s.Init(file, &reader) // reads first rune

	line, column := 1, 1

	for i := 0; i < len(str); i++ {
		expectRuneAtPos(t, &s, str, offset, i)
		if str[i] == '\n' {
			line++
			column = 1
		} else {
			column++
		}
		expectPosition(t, file, s.endpos, line, column)
		s.next()
	}
	if s.ch != runeEOF {
		t.Errorf("Scanner found '%c', expecting EOF", s.ch)
	}
}

func expectRuneAtPos(t *testing.T, s *Scanner, str string, offset int, i int) {
	if s.endpos-1 != offset+i || s.ch != rune(str[i]) {
		t.Errorf("Scanner found '%c' at Pos 0x%x, expecting '%c' at Pos 0x%x", s.ch, s.endpos-1, str[i], offset+i)
	}
}

func expectPosition(t *testing.T, file *token.File, offset int, line int, column int) {
	pos := file.Position(file.Pos(offset))
	if pos.Offset != offset || pos.Line != line || pos.Column != column {
		t.Errorf("Scanner is at %#v, expecting {Offset: %d, Line: %d, Column: %d}",
			pos, offset, line, column)
	}
}

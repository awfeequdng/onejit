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
 * op.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package common

import (
	"strconv"
)

type Op uint8 // intentionally same values as go.Token

const (
	BADOP Op = iota

	ADD Op = 9 + iota // +
	SUB               // -
	MUL               // *
	QUO               // /
	REM               // %

	AND     // &
	OR      // |
	XOR     // ^
	SHL     // <<
	SHR     // >>
	AND_NOT // &^

	ADD_ASSIGN // +=
	SUB_ASSIGN // -=
	MUL_ASSIGN // *=
	QUO_ASSIGN // /=
	REM_ASSIGN // %=

	AND_ASSIGN     // &=
	OR_ASSIGN      // |=
	XOR_ASSIGN     // ^=
	SHL_ASSIGN     // <<=
	SHR_ASSIGN     // >>=
	AND_NOT_ASSIGN // &^=

	LAND // &&
	LOR  // ||
	_    // ARROW // <-
	INC  // ++
	DEC  // --

	EQL    // ==
	LSS    // <
	GTR    // >
	ASSIGN // =
	NOT    // !

	NEQ // !=
	LEQ // <=
	GEQ // >=

	_       // DEFINE   // :=
	_       // ELLIPSIS // ...
	CAST    // LPAREN   // (
	BRACKET // LBRACK   // [
	_
	_
	FIELD // .

	opLo = BADOP
	opHi = FIELD
)

var opstring = [...]string{
	BADOP:          "BADOP",
	ADD:            "+",
	SUB:            "-",
	MUL:            "*",
	QUO:            "/",
	REM:            "%",
	AND:            "&",
	OR:             "|",
	XOR:            "^",
	SHL:            "<<",
	SHR:            ">>",
	AND_NOT:        "&^",
	ADD_ASSIGN:     "+=",
	SUB_ASSIGN:     "-=",
	MUL_ASSIGN:     "*=",
	QUO_ASSIGN:     "/=",
	REM_ASSIGN:     "%=",
	AND_ASSIGN:     "&=",
	OR_ASSIGN:      "|=",
	XOR_ASSIGN:     "^=",
	SHL_ASSIGN:     "<<=",
	SHR_ASSIGN:     ">>=",
	AND_NOT_ASSIGN: "&^=",
	LAND:           "&&",
	LOR:            "||",
	INC:            "++",
	DEC:            "--",
	EQL:            "==",
	LSS:            "<",
	GTR:            ">",
	ASSIGN:         "=",
	NOT:            "!",
	NEQ:            "!=",
	LEQ:            "<=",
	GEQ:            ">=",
	CAST:           "CAST",
	BRACKET:        "[]",
	FIELD:          ".",
}

func (op Op) String() string {
	var s string
	if op >= opLo && op <= opHi {
		s = opstring[op]
	} else {
		s = "Op(" + strconv.Itoa(int(op)) + ")"
	}
	return s

}

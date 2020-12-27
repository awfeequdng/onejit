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
 * op.go
 *
 *  Created on Jan 23, 2019
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"strconv"
)

type (
	Op uint8
)

// intentionally same values as go.Token
const (
	BADOP Op = iota
	_
	NOOP // COMMENT
	_
	_
	_
	_
	_
	_
	ADD // +
	SUB // -
	MUL // *
	QUO // /
	REM // %

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
	LNOT   // !

	NEQ  // !=
	LEQ  // <=
	GEQ  // >=
	ZERO // DEFINE // :=
	_
	CAST    // LPAREN   // (
	BRACKET // LBRACK   // [
	_
	_
	FIELD // DOT // .

	JUMP    Op = 73 // GOTO
	JUMP_IF Op = 74 // IF
	RET     Op = 80 // RETURN

	cmp Op = 81 // arch-specific: set arch flags to comparison result
	jeq Op = 82 // arch-specific: jump if equal
	jlt Op = 83 // arch-specific: jump if less
	jgt Op = 84 // arch-specific: jump if greater
	jne Op = 85 // arch-specific: jump if not equal
	jle Op = 86 // arch-specific: jump if less or equal
	jge Op = 87 // arch-specific: jump if greater or equal

	NEG  = SUB // -
	INV  = XOR // ^
	STAR = MUL // *

	opLo = ADD
	opHi = jge
)

var opstring = [...]string{
	BADOP:          "BADOP",
	NOOP:           "NOOP",
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
	LNOT:           "!",
	NEQ:            "!=",
	LEQ:            "<=",
	GEQ:            ">=",
	CAST:           "CAST",
	BRACKET:        "[]",
	FIELD:          ".",
	JUMP:           "JUMP",
	JUMP_IF:        "JUMP_IF",
	RET:            "RET",
	cmp:            "CMP",
	jeq:            "JEQ",
	jlt:            "JLT",
	jgt:            "JGT",
	jne:            "JNE",
	jle:            "JLE",
	jge:            "JGE",
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

func (op Op) IsCommutative() bool {
	var ret bool
	switch op {
	case ADD, MUL, AND, OR, XOR, EQL, NEQ:
		ret = true
	}
	return ret
}

func (op Op) IsComparison() bool {
	var ret bool
	switch op {
	case EQL, LSS, GTR, NEQ, LEQ, GEQ:
		ret = true
	}
	return ret
}

func notComparison(op Op) Op {
	switch op {
	case EQL:
		op = NEQ
	case NEQ:
		op = EQL
	case LSS:
		op = GEQ
	case GTR:
		op = LEQ
	case LEQ:
		op = GTR
	case GEQ:
		op = LSS
	default:
		Errorf("bad notComparison op: have %v, want EQL,NEQ,LSS,GTR,LEQ or GEQ", op)
	}
	return op
}

func swapComparison(op Op) Op {
	switch op {
	case EQL, NEQ:
		break
	case LSS:
		op = GTR
	case GTR:
		op = LSS
	case LEQ:
		op = GEQ
	case GEQ:
		op = LEQ
	default:
		Errorf("bad swapComparison op: have %v, want EQL,NEQ,LSS,GTR,LEQ or GEQ", op)
	}
	return op
}

func toConditionalJump(op Op) Op {
	switch op {
	case EQL:
		return jeq
	case NEQ:
		return jne
	case LSS:
		return jlt
	case GTR:
		return jgt
	case LEQ:
		return jle
	case GEQ:
		return jge
	default:
		Errorf("bad toConditionalJump op: have %v, want EQL,NEQ,LSS,GTR,LEQ or GEQ", op)
	}
	return op
}

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

package internal

import (
	"strconv"
)

type (
	Op uint8
)

// intentionally same values as go.Token
const (
	BADOP Op = iota // ILLEGAL
	_               // EOF
	NOOP            // COMMENT
	_               // IDENT
	_               // INT
	_               // FLOAT
	_               // IMAG
	_               // CHAR
	_               // STRING

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

	NEQ     // !=
	LEQ     // <=
	GEQ     // >=
	ZERO    // DEFINE    // :=
	_       // ELLIPSIS  // ...
	CAST    // LPAREN    // (
	BRACKET // LBRACK    // [
	BLOCK   // LBRACE    // {
	_       // COMMA     // ,
	FIELD   // PERIOD    // .
	EXPR    // RPAREN    // ) // returned by ExprStmt.Op()
	_       // RBRACK    // ]
	_       // RBRACE    // }
	_       // SEMICOLON // ;
	_       // COLON     // :
	BREAK
	CASE
	_           // CHAN
	CONST       // returned by Const.Op() and Label.Op()
	CONTINUE    //
	_           // DEFAULT
	_           // DEFER
	FALLTHROUGH //
	FOR         //
	_           // FUNC
	_           // GO
	JUMP        // GOTO
	IF          //
	_           // IMPORT
	_           // INTERFACE
	_           // MAP
	_           // PACKAGE
	_           // RANGE
	RET         // RETURN
	_           // SELECT
	_           // STRUCT
	SWITCH      //
	_           // TYPE
	VAR         // returned by Reg.Op() and Mem.Op()
	_
	_
	_
	_
	_
	_
	_
	_
	_
	_
	_
	_
	_
	_
	_
	JUMP_IF // 100
	_
	_
	_
	_
	_
	_
	ARCH_JEQ // arch-specific: jump if equal
	ARCH_JLT // arch-specific: jump if less
	ARCH_JGT // arch-specific: jump if greater
	ARCH_JNE // arch-specific: jump if not equal
	ARCH_JLE // arch-specific: jump if less or equal
	ARCH_JGE // arch-specific: jump if greater or equal
	ARCH_JZ  // arch-specific: jump if zero
	ARCH_JNZ // arch-specific: jump if not zero
	_
	ARCH_CMP // arch-specific: set arch flags to result of comparison
	X86_TEST // amd64, x86 only: set arch flags to result of bitwise and
	X86_LEA  // amd64, x86 only: LEA

	opMax

	NEG  = SUB // -
	INV  = XOR // ^
	STAR = MUL // *

	opLo = BADOP
	opHi = opMax - 1
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
	BLOCK:          "{}",
	FIELD:          ".",
	BREAK:          "BREAK",
	CASE:           "CASE",
	CONTINUE:       "CONTINUE",
	FALLTHROUGH:    "FALLTHROUGH",
	FOR:            "FOR",
	JUMP:           "JUMP",
	IF:             "IF",
	RET:            "RET",
	SWITCH:         "SWITCH",
	JUMP_IF:        "JUMP_IF",
	ARCH_JEQ:       "JEQ",
	ARCH_JLT:       "JLT",
	ARCH_JGT:       "JGT",
	ARCH_JNE:       "JNE",
	ARCH_JLE:       "JLE",
	ARCH_JGE:       "JGE",
	ARCH_JZ:        "JZ",
	ARCH_JNZ:       "JNZ",
	ARCH_CMP:       "CMP",
	X86_TEST:       "X86_TEST",
	X86_LEA:        "X86_LEA",
}

func (op Op) String() string {
	var s string
	if op >= opLo && op <= opHi {
		s = opstring[op]
	}
	if len(s) == 0 {
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

func SwapComparison(op Op) Op {
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

func ToConditionalJump(op Op) Op {
	switch op {
	case EQL:
		return ARCH_JEQ
	case NEQ:
		return ARCH_JNE
	case LSS:
		return ARCH_JLT
	case GTR:
		return ARCH_JGT
	case LEQ:
		return ARCH_JLE
	case GEQ:
		return ARCH_JGE
	default:
		Errorf("bad ToConditionalJump op: have %v, want EQL,NEQ,LSS,GTR,LEQ or GEQ", op)
	}
	return op
}

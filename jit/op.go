/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * op.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/onejit/jit/internal"
)

const (
	BADOP = internal.BADOP // ILLEGAL
	_                      // EOF
	NOOP  = internal.NOOP  // COMMENT
	_                      // IDENT
	_                      // INT
	_                      // FLOAT
	_                      // IMAG
	_                      // CHAR
	_                      // STRING

	ADD = internal.ADD // +
	SUB = internal.SUB // -
	MUL = internal.MUL // *
	QUO = internal.QUO // /
	REM = internal.REM // %

	AND     = internal.AND     // &
	OR      = internal.OR      // |
	XOR     = internal.XOR     // ^
	SHL     = internal.SHL     // <<
	SHR     = internal.SHR     // >>
	AND_NOT = internal.AND_NOT // &^

	ADD_ASSIGN = internal.AND_ASSIGN // +=
	SUB_ASSIGN = internal.SUB_ASSIGN // -=
	MUL_ASSIGN = internal.MUL_ASSIGN // *=
	QUO_ASSIGN = internal.QUO_ASSIGN // /=
	REM_ASSIGN = internal.REM_ASSIGN // %=

	AND_ASSIGN     = internal.AND_ASSIGN     // &=
	OR_ASSIGN      = internal.OR_ASSIGN      // |=
	XOR_ASSIGN     = internal.XOR_ASSIGN     // ^=
	SHL_ASSIGN     = internal.SHL_ASSIGN     // <<=
	SHR_ASSIGN     = internal.SHR_ASSIGN     // >>=
	AND_NOT_ASSIGN = internal.AND_NOT_ASSIGN // &^=

	LAND = internal.LAND // &&
	LOR  = internal.LOR  // ||
	_                    // ARROW // <-
	INC  = internal.INC  // ++
	DEC  = internal.DEC  // --

	EQL    = internal.EQL    // ==
	LSS    = internal.LSS    // <
	GTR    = internal.GTR    // >
	ASSIGN = internal.ASSIGN // =
	LNOT   = internal.LNOT   // !

	NEQ         = internal.NEQ     // !=
	LEQ         = internal.LEQ     // <=
	GEQ         = internal.GEQ     // >=
	ZERO        = internal.ZERO    // DEFINE    // :=
	_                              // ELLIPSIS  // ...
	CAST        = internal.CAST    // LPAREN    // (
	BRACKET     = internal.BRACKET // LBRACK    // [
	BLOCK       = internal.BLOCK   // LBRACE    // {
	_                              // COMMA     // ,
	FIELD       = internal.FIELD   // PERIOD    // .
	EXPR        = internal.EXPR    // RPAREN    // ) // returned by ExprStmt.Op()
	_                              // RBRACK    // ]
	_                              // RBRACE    // }
	_                              // SEMICOLON // ;
	_                              // COLON     // :
	BREAK       = internal.BREAK
	CASE        = internal.CASE
	_                               // CHAN
	CONST       = internal.CONST    // returned by Const.Op() and Label.Op()
	CONTINUE    = internal.CONTINUE //
	_                               // DEFAULT
	_                               // DEFER
	FALLTHROUGH = internal.FALLTHROUGH
	FOR         = internal.FOR    //
	_                             // FUNC
	_                             // GO
	JUMP        = internal.JUMP   // GOTO
	IF          = internal.IF     //
	_                             // IMPORT
	_                             // INTERFACE
	_                             // MAP
	_                             // PACKAGE
	_                             // RANGE
	RET         = internal.RET    // RETURN
	_                             // SELECT
	_                             // STRUCT
	SWITCH      = internal.SWITCH //
	_                             // TYPE
	VAR         = internal.VAR    // returned by Reg.Op() and Mem.Op()
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
	JUMP_IF = internal.JUMP_IF
	_
	_
	_
	_
	_
	_
	ARCH_JEQ = internal.ARCH_JEQ // arch-specific: jump if equal
	ARCH_JLT = internal.ARCH_JLT // arch-specific: jump if less
	ARCH_JGT = internal.ARCH_JGT // arch-specific: jump if greater
	ARCH_JNE = internal.ARCH_JNE // arch-specific: jump if not equal
	ARCH_JLE = internal.ARCH_JLE // arch-specific: jump if less or equal
	ARCH_JGE = internal.ARCH_JGE // arch-specific: jump if greater or equal
	ARCH_JZ  = internal.ARCH_JZ  // arch-specific: jump if zero
	ARCH_JNZ = internal.ARCH_JNZ // arch-specific: jump if not zero
	_
	ARCH_CMP  = internal.ARCH_CMP  // arch-specific: set arch flags to result of comparison
	X86_TEST  = internal.X86_TEST  // arch-specific: set arch flags to result of bitwise and
	X86_TESTZ = internal.X86_TESTZ // arch-specific: set arch flags to result of bitwise and

	NEG        = internal.NEG        // -
	INV        = internal.INV        // ^
	STAR       = internal.STAR       // *
	NEG_ASSIGN = internal.NEG_ASSIGN // x = -x
)

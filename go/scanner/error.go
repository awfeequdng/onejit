/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * error.go
 *
 *  Created on: Mar 22, 2021
 *      Author: Massimiliano Ghilardi
 */

package scanner

import (
	"github.com/cosmos72/onejit/go/token"
)

type Error struct {
	Pos token.Position
	Msg string
}

func (e *Error) Error() string {
	if len(e.Pos.Filename) == 0 || !e.Pos.IsValid() {
		return e.Msg
	}
	return e.Pos.String() + ": " + e.Msg
}

var (
	errInvalidCharacter = "invalid character"
	errInvalidOperator  = "invalid operator"
	errInvalidUtf8      = "invalid UTF-8 sequence"
	errUnexpectedEOF    = "unexpected EOF"

	// number-related error messages
	errIntBadUnderscore          = "'_' must separate successive digits"
	errSyntaxErrorUnexpectedChar = "syntax error: unexpected character"

	errNumberHasNodigits = []string{
		base2:  "binary literal has no digits",
		base8:  "octal literal has no digits",
		base16: "hexadecimal literal has no digits",
		base10: "decimal literal has no digits",
	}

	errIntBadDigit = []string{
		base2:  "invalid digit in binary literal",
		base8:  "invalid digit in octal literal",
		base16: "invalid digit in hexadecimal literal",
		base10: "invalid digit in decimal literal",
	}

	errFloatExponentHasNoDigits = "exponent has no digits"
	errHexFloatNoPrefix         = "hexadecimal float requires \"0x\" prefix"
	errHexFloatNoExponent       = "hexadecimal float requires exponent"

	// string-related errors
	errStringContainsNewline = []string{
		"newline in string literal",
		"newline in rune literal",
	}
	errStringUnterminated = []string{
		"string literal not terminated",
		"rune literal not terminated",
		"raw string literal not terminated",
	}
	errEscapeUnknown          = "unknown escape"
	errEscapeHexInvalidChar   = "invalid character in hexadecimal escape"
	errEscapeOctalInvalidChar = "invalid character in octal escape"
	errEscapeInvalidCodepoint = "escape is invalid Unicode code point"
	errRuneEmpty              = "empty rune literal or unescaped '"
	errRuneTooLong            = "more than one character in rune literal"
)

func (u *utf8Reader) error1(err *Error) {
	u.errors = append(u.errors, err)
	if len(u.errors) >= 10 {
		u.unread = runeTOO_MANY_ERRORS
	}
}

func (u *utf8Reader) error(msg string) bool {
	file := u.file
	u.error1(&Error{
		Pos: file.Position(file.Pos(u.pos)),
		Msg: msg,
	})
	return false
}

func (s *Scanner) invalid(msg string) bool {
	s.tok = token.ILLEGAL
	if !isSpace(s.ch) {
		s.add()
	}
	s.lit = s.getString()
	s.next()
	return s.error(msg)
}

func (s *Scanner) Error(pos token.Pos, msg string) *Error {
	err := &Error{
		Pos: s.file.Position(pos),
		Msg: msg,
	}
	s.error1(err)
	return err
}

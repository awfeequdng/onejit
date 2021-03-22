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
	"errors"

	"github.com/cosmos72/onejit/go/token"
)

var (
	errInvalidCharacter = errors.New("invalid character")
	errInvalidOperator  = errors.New("invalid operator")
	errInvalidUtf8      = errors.New("invalid UTF-8 sequence")
	errUnexpectedEOF    = errors.New("unexpected EOF")

	// number-related errors
	errIntBadUnderscore          = errors.New("'_' must separate successive digits")
	errSyntaxErrorUnexpectedChar = errors.New("syntax error: unexpected character")

	errNumberHasNodigits = []error{
		base2:  errors.New("binary literal has no digits"),
		base8:  errors.New("octal literal has no digits"),
		base16: errors.New("hexadecimal literal has no digits"),
		base10: errors.New("decimal literal has no digits"),
	}

	errIntBadDigit = []error{
		base2:  errors.New("invalid digit in binary literal"),
		base8:  errors.New("invalid digit in octal literal"),
		base16: errors.New("invalid digit in hexadecimal literal"),
		base10: errors.New("invalid digit in decimal literal"),
	}

	errFloatExponentHasNoDigits = errors.New("exponent has no digits")
	errHexFloatNoPrefix         = errors.New("hexadecimal float requires \"0x\" prefix")
	errHexFloatNoExponent       = errors.New("hexadecimal float requires exponent")

	// string-related errors
	errStringContainsNewline = []error{
		errors.New("newline in string literal"),
		errors.New("newline in rune literal"),
	}
	errStringUnterminated = []error{
		errors.New("string literal not terminated"),
		errors.New("rune literal not terminated"),
		errors.New("raw string literal not terminated"),
	}
	errEscapeUnknown          = errors.New("unknown escape")
	errEscapeHexInvalidChar   = errors.New("invalid character in hexadecimal escape")
	errEscapeOctalInvalidChar = errors.New("invalid character in octal escape")
	errEscapeInvalidCodepoint = errors.New("escape is invalid Unicode code point")
	errRuneEmpty              = errors.New("empty rune literal or unescaped '")
	errRuneTooLong            = errors.New("more than one character in rune literal")
)

func (s *Scanner) error(err error) bool {
	s.err = append(s.err, err)
	return false
}

func (s *Scanner) invalid(err error) bool {
	s.tok = token.ILLEGAL
	if !isSpace(s.ch) {
		s.add()
	}
	s.lit = s.getString()
	s.next()
	return s.error(err)
}

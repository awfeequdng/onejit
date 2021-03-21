/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * number.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package scanner

import (
	"errors"

	"github.com/cosmos72/onejit/go/token"
)

var errIntBadUnderscore = errors.New("'_' must separate successive digits")
var errSyntaxErrorUnexpectedChar = errors.New("syntax error: unexpected character")

var errIntHasNodigits = []error{
	base2:  errors.New("binary literal has no digits"),
	base8:  errors.New("octal literal has no digits"),
	base16: errors.New("hexadecimal literal has no digits"),
	base10: errors.New("decimal literal has no digits"),
}

var errIntBadDigit = []error{
	base2:  errors.New("invalid digit in binary literal"),
	base8:  errors.New("invalid digit in octal literal"),
	base16: errors.New("invalid digit in hexadecimal literal"),
	base10: errors.New("invalid digit in decimal literal"),
}

func (s *Scanner) scanNumber() {
	ch := s.ch
	if !isDecimalDigit(ch) {
		return
	}
	b := &s.builder
	b.Reset()

	base := base10
	if ch == '0' {
		b.WriteRune(ch)
		addRune := true
		switch ch = s.next(); ch {
		case 'B', 'b':
			base = base2
		case 'O', 'o':
			base = base8
		case 'X', 'x':
			base = base16
		default:
			base = base8
			addRune = false
		}
		if addRune {
			b.WriteRune(ch)
			ch = s.next()
		}
	}
	s.scanInt(ch, base)
}

func (s *Scanner) scanInt(ch rune, base intbase) {
	b := &s.builder
	lastIsUnderscore := false
	for {
		//! TODO: also parse floating point literals
		if isSpace(ch) || isOperator(ch) {
			break
		} else if isIntDigit(ch, base) {
			lastIsUnderscore = false
		} else if isIntDigit(ch, base10) {
			s.invalid(errIntBadDigit[base])
			return
		} else if ch == '_' {
			if lastIsUnderscore {
				s.invalid(errIntBadUnderscore)
				return
			}
			lastIsUnderscore = true
		} else {
			s.invalid(errSyntaxErrorUnexpectedChar)
			return
		}
		b.WriteRune(ch)
		ch = s.next()
	}
	if lastIsUnderscore {
		s.invalid(errIntBadUnderscore)
		return
	}
	str := b.String()
	if s.checkValidInt(str, base) {
		s.Tok = token.INT
		s.Lit = str
		s.next()
	}
}

func (s *Scanner) checkValidInt(str string, base intbase) bool {
	if len(str) == 1 || len(str) > 2 || str[0] != '0' {
		return true
	}
	last := rune(str[1])
	if isIntDigit(last, base) {
		return true
	}
	return s.invalid(errIntHasNodigits[base])
}

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

var (
	errSyntaxErrorUnterminatedRuneOrString = errors.New("syntax error: unterminated rune or string literal")
	errUnknownEscape                       = errors.New("unknown escape")
	errInvalidCharInHexEscape              = errors.New("invalid character in hexadecimal escape")
)

func (s *Scanner) scanRune() {
	ch := s.ch
	if ch == '\'' {
		s.scanRuneOrString(ch)
	}
}

func (s *Scanner) scanString() {
	ch := s.ch
	if ch == '"' {
		s.scanRuneOrString(ch)
	}
}

func (s *Scanner) scanRuneOrString(delim rune) {
	b := &s.builder
	b.Reset()
	b.WriteRune(delim)
	escape := false
	for {
		ch := s.next()
		if ch == runeEOF {
			s.invalid(errSyntaxErrorUnterminatedRuneOrString)
			return
		} else if !escape {
			if ch == delim {
				break
			} else if ch == '\\' {
				escape = true
				continue
			}
		} else /*escape*/ {
			escape = false
			switch ch {
			case 'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', delim:
			case 'U':
				s.scanUnicodeHexDigits(ch, 8)
				continue
			case 'u':
				s.scanUnicodeHexDigits(ch, 4)
				continue
			case 'x':
				s.scanUnicodeHexDigits(ch, 2)
				continue
			default:
				s.error(errUnknownEscape)
			}
		}
		b.WriteRune(ch)
	}
	if delim == '"' {
		s.Tok = token.STRING
	} else {
		s.Tok = token.CHAR
	}
	b.WriteRune(delim)
	s.Lit = b.String()
}

func (s *Scanner) scanUnicodeHexDigits(initial rune, n uint) {
	b := &s.builder
	b.WriteByte('\\')
	b.WriteRune(initial)
	for i := uint(0); i < n; i++ {
		ch := s.next()
		if ch == runeEOF {
			s.invalid(errSyntaxErrorUnterminatedRuneOrString)
			return
		} else if !isHexDigit(ch) {
			s.error(errInvalidCharInHexEscape)
		}
		b.WriteRune(ch)
	}
}

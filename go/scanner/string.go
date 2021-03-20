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

func (s *Scanner) scanString() {
	ch := s.ch
	if ch == '"' {
		s.scanRuneOrString(ch)
	}
}

func (s *Scanner) scanRuneOrString(runeEnd rune) {
	b := &s.builder
	b.Reset()
	escape := false
	for {
		ch := s.next()
		if ch == runeEOF {
			panic(errSyntaxErrorUnterminatedRuneOrString)
		} else if !escape {
			if ch == runeEnd {
				break
			} else if ch == '\\' {
				escape = true
				continue
			}
		} else /*escape*/ {
			escape = false
			switch ch {
			case 'U':
				s.scanUnicodeHexDigits(8)
			case 'a':
				ch = '\a'
			case 'b':
				ch = '\b'
			case 'f':
				ch = '\f'
			case 'n':
				ch = '\n'
			case 'r':
				ch = '\r'
			case 't':
				ch = '\r'
			case 'u':
				s.scanUnicodeHexDigits(2)
			case 'v':
				ch = '\v'
			case 'x':
				s.scanUnicodeHexDigits(2)
			case '\\':
				ch = '\\'
			case runeEnd: // either " or '
				ch = runeEnd
			default:
				panic(errUnknownEscape)
			}
		}
		b.WriteRune(ch)
	}
	if runeEnd == '"' {
		s.Tok = token.STRING
	} else {
		s.Tok = token.CHAR
	}
	s.Lit = b.String()
}

func (s *Scanner) scanUnicodeHexDigits(n uint) {
	x := 0
	for i := uint(0); i < n; i++ {
		ch := s.next()
		if ch == runeEOF {
			panic(errSyntaxErrorUnterminatedRuneOrString)
		} else if !isHexDigit(ch) {
			panic(errInvalidCharInHexEscape)
		}
		x = x | hexDigitToInt(ch)<<(i*8)
	}
	s.builder.WriteRune(rune(x))
}

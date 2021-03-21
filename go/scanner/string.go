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
	"unicode/utf8"

	"github.com/cosmos72/onejit/go/token"
)

var (
	errNewlineInString = []error{
		errors.New("newline in string literal"),
		errors.New("newline in rune literal"),
	}
	errSyntaxErrorUnterminatedString = []error{
		errors.New("syntax error: unterminated string literal"),
		errors.New("syntax error: unterminated rune literal"),
	}
	errUnknownEscape            = errors.New("unknown escape")
	errInvalidCharInHexEscape   = errors.New("invalid character in hexadecimal escape")
	errEscapeIsInvalidCodepoint = errors.New("escape is invalid Unicode code point")
)

type strKind rune

func (kind strKind) rune() rune {
	return rune(kind)
}

func (kind strKind) index() int {
	if kind == '"' {
		return 0
	}
	return 1
}

func (kind strKind) token() token.Token {
	if kind == '"' {
		return token.STRING
	}
	return token.CHAR
}

////////////////////////////////////////////////////////////////////////////////

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
	errnum := len(s.err)
	kind := strKind(delim)
	cont := true
	escape := false
	for cont {
		ch := s.next()
		if ch == runeEOF {
			s.invalid(errSyntaxErrorUnterminatedString[kind.index()])
			return
		} else if ch == '\n' {
			s.invalid(errNewlineInString[kind.index()])
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
				cont = s.scanUnicodeHexDigits(kind, ch, 8)
				continue
			case 'u':
				cont = s.scanUnicodeHexDigits(kind, ch, 4)
				continue
			case 'x':
				cont = s.scanUnicodeHexDigits(kind, ch, 2)
				continue
			default:
				s.error(errUnknownEscape)
			}
			b.WriteByte('\\')
		}
		b.WriteRune(ch)
	}
	if len(s.err) > errnum {
		s.Tok = token.ILLEGAL
	} else {
		s.Tok = kind.token()
	}
	if cont {
		b.WriteRune(delim)
	}
	s.Lit = b.String()
}

func (s *Scanner) scanUnicodeHexDigits(kind strKind, initial rune, n uint) bool {
	b := &s.builder
	b.WriteByte('\\')
	b.WriteRune(initial)
	var x uint32
	for n != 0 {
		n--
		ch := s.next()
		if ch == runeEOF {
			return s.error(errSyntaxErrorUnterminatedString[kind.index()])
		} else if !isHexDigit(ch) {
			s.error(errInvalidCharInHexEscape)
		} else {
			x |= uint32(hexDigitToInt(ch)) << (n * 4)
		}
		b.WriteRune(ch)
	}
	if (initial == 'U' || initial == 'u') && !utf8.ValidRune(rune(x)) {
		s.error(errEscapeIsInvalidCodepoint)
	}
	return true
}

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
	size := 0
	kind := strKind(delim)
	cont := true
	escape := false
	for cont {
		ch := s.next()
		if ch == runeEOF {
			s.invalid(errStringUnterminated[kind.index()])
			return
		} else if ch == '\n' {
			s.invalid(errStringContainsNewline[kind.index()])
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
				size++
				continue
			case 'u':
				cont = s.scanUnicodeHexDigits(kind, ch, 4)
				size++
				continue
			case 'x':
				cont = s.scanUnicodeHexDigits(kind, ch, 2)
				size++
				continue
			default:
				if isOctalDigit(ch) {
					cont = s.scanUnicodeOctalDigits(kind, ch, 3, 0xff)
					size++
					continue
				}
				s.error(errEscapeUnknown)
			}
			b.WriteByte('\\')
		}
		b.WriteRune(ch)
		size++
	}
	if len(s.err) > errnum {
		s.Tok = token.ILLEGAL
	} else if delim == '\'' && size != 1 {
		s.Tok = token.ILLEGAL
		if size == 0 {
			s.error(errRuneEmpty)
		} else {
			s.error(errRuneTooLong)
		}
	} else {
		s.Tok = kind.token()
	}
	if cont {
		b.WriteRune(delim)
	}
	s.Lit = b.String()
	s.next()
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
			return s.error(errStringUnterminated[kind.index()])
		} else if !isHexDigit(ch) {
			s.error(errEscapeHexInvalidChar)
		} else {
			x |= uint32(hexDigitToInt(ch)) << (n * 4)
		}
		b.WriteRune(ch)
	}
	if (initial == 'U' || initial == 'u') && !utf8.ValidRune(rune(x)) {
		s.error(errEscapeInvalidCodepoint)
	}
	return true
}

func (s *Scanner) scanUnicodeOctalDigits(kind strKind, ch rune, n uint, max uint32) bool {
	b := &s.builder
	b.WriteByte('\\')
	var x uint32
	for n != 0 {
		n--
		if ch == runeEOF {
			return s.error(errStringUnterminated[kind.index()])
		} else if !isOctalDigit(ch) {
			s.error(errEscapeOctalInvalidChar)
		} else {
			x |= uint32(ch-'0') << (n * 3)
		}
		b.WriteRune(ch)
		if n != 0 {
			ch = s.next()
		}
	}
	if x > max {
		s.error(errEscapeInvalidCodepoint)
	}
	return true
}

////////////////////////////////////////////////////////////////////////////////

func (s *Scanner) scanRawString() {
	ch := s.ch
	if ch != '`' {
		return
	}
	b := &s.builder
	b.WriteRune(ch)
	for {
		ch = s.next()
		if ch == runeEOF {
			s.invalid(errStringUnterminated[2])
			return
		} else if ch == '\r' {
			// Go specs: Carriage return characters ('\r') inside raw string
			// literals are discarded from the raw string value.
			continue
		} else {
			b.WriteRune(ch)
			if ch == '`' {
				break
			}
		}
	}
	s.Tok = token.STRING
	s.Lit = b.String()
	s.next()
}

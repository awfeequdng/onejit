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
	"github.com/cosmos72/onejit/go/token"
)

func (s *Scanner) scanNumber() {
	ch := s.ch
	if !isDecimalDigit(ch) {
		return
	}

	base := baseAny
	if ch == '0' {
		s.addRune(ch)
		switch ch = s.next(); ch {
		case 'B', 'b':
			base = base2
		case 'O', 'o':
			base = base8
		case 'X', 'x':
			base = base16
		default:
		}
	}
	if base != baseAny {
		s.scanInt(base)
	} else {
		s.scanIntOrFloat(base10, 0)
	}
}

func (s *Scanner) scanInt(base intbase) {
	s.add()
	s.next()
	if !s.scanIntDigits(base) {
		return
	}
	ch := s.ch
	if base == base16 && (ch == '.' || ch == 'p' || ch == 'P') {
		// hexadecimal float
		s.scanIntOrFloat(base, ch)
		return
	}
	if !isSpace(ch) && !isOperator(ch) {
		s.invalid(errSyntaxErrorUnexpectedChar)
		return
	}
	str := s.getString()
	if s.quickCheckValidInt(str, base) {
		s.setResult(token.INT)
		s.next()
	}
}

func (s *Scanner) scanIntDigits(base intbase) bool {
	ch := s.ch
	lastIsUnderscore := false
	for {
		if isIntDigit(ch, base) {
			lastIsUnderscore = false

		} else if isIntDigit(ch, base10) {
			return s.invalid(errIntBadDigit[base])

		} else if ch == '_' {
			if lastIsUnderscore {
				return s.invalid(errIntBadUnderscore)
			}
			lastIsUnderscore = true
		} else {
			break
		}
		s.addRune(ch)
		ch = s.next()
	}
	if lastIsUnderscore {
		return s.invalid(errIntBadUnderscore)
	}
	return true
}

func (s *Scanner) scanDecimalDigits() bool {
	return s.scanIntDigits(base10)
}

// scan characters after an initial digit [0-9]
func (s *Scanner) scanIntOrFloat(base intbase, prevChar rune) {
	tok := token.INT
	if prevChar == 0 && !s.scanIntDigits(base) {
		return
	}

	ch := s.ch
	if prevChar == '.' || ch == '.' {
		s.addRune('.')
		// we may have already read next char after '.'
		if ch == '.' {
			s.next()
		}
		if !s.scanIntDigits(base) {
			return
		}
		tok = token.FLOAT
		ch = s.ch
	}

	exponent := (base == base10 && (ch == 'e' || ch == 'E')) ||
		(base == base16 && (ch == 'p' || ch == 'P'))

	if exponent {
		if !s.scanFloatExponent() {
			return
		}
		tok = token.FLOAT
		ch = s.ch
	}
	if ch == 'i' && s.getString() != "." {
		s.add()
		ch = s.next()
		tok = token.IMAG
	}
	if tok == token.FLOAT && base == base16 {
		if !exponent {
			s.invalid(errHexFloatNoExponent)
			return
		} else if !s.checkValidHexFloat(s.getString()) {
			return
		}
	}
	if !isSpace(ch) && !isOperator(ch) {
		s.invalid(errSyntaxErrorUnexpectedChar)
		return
	}
	if tok == token.INT {
		str := s.getString()
		if len(str) > 0 && str[0] == '0' {
			// tok == INT and initial digit is 0
			// so check for valid octal number
			if !s.checkValidInt(str, base8) {
				return
			}
		}
	}
	s.setResult(tok)
	s.next()
}

func (s *Scanner) scanFloatExponent() bool {
	s.add()
	ch := s.next()
	if ch == '+' || ch == '-' {
		s.addRune(ch)
		ch = s.next()
	}
	if !isDecimalDigit(ch) {
		s.invalid(errFloatExponentHasNoDigits)
		return false
	}
	s.scanDecimalDigits()
	return true
}

func (s *Scanner) quickCheckValidInt(str string, base intbase) bool {
	if len(str) == 1 || len(str) > 2 || str[0] != '0' {
		return true
	}
	last := rune(str[1])
	if isIntDigit(last, base) {
		return true
	}
	return s.invalid(errNumberHasNodigits[base])
}

func (s *Scanner) checkValidInt(str string, base intbase) bool {
	n := 0
	for _, ch := range str {
		if isIntDigit(ch, base) {
			n++
		} else if isIntDigit(ch, base10) {
			return s.invalid(errIntBadDigit[base])
		} else if ch != '_' {
			// base prefix 'b' 'o' 'x' ...
			n = 0
		}
	}
	if n == 0 {
		return s.invalid(errNumberHasNodigits[base])
	}
	return true
}

func (s *Scanner) checkValidHexFloat(str string) bool {
	n := len(str)
	if n < 2 || str[0] != '0' || lower(str[1]) != 'x' {
		return s.invalid(errHexFloatNoPrefix) // should never happen

	} else if n < 3 || lower(str[2]) == 'p' {
		return s.invalid(errNumberHasNodigits[base16])

	} else if lower(str[2]) == '.' {
		// "0x." must be followed by a fractional digit
		if n < 4 || !isHexDigit(rune(str[3])) {
			return s.invalid(errNumberHasNodigits[base16])
		}
	}
	return true
}

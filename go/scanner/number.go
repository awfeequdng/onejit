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

func (s *Scanner) number() {
	ch := s.ch
	if !isDecimalDigit(ch) {
		return
	}
	b := &s.builder
	b.Reset()

	base := base10
	if ch == '0' {
		b.WriteRune(ch)
		switch ch = s.next(); ch {
		case 'B', 'b':
			base = base2
		case 'O', 'o':
			base = base8
		case 'X', 'x':
			base = base16
		default:
			base = base8
		}
	}
	s.integer(ch, base)
}

func (s *Scanner) integer(ch rune, base intbase) {
	b := &s.builder
	lastIsUnderscore := false
	if isSpace(ch) || isOperator(ch) {
		s.Tok = token.INT
		s.Lit = "0"
		return
	}
	lastIsUnderscore = ch == '_'
	if !lastIsUnderscore {
		b.WriteRune(ch)
	}
	for {
		ch := s.next()
		if isIntDigit(ch, base) {
			b.WriteRune(ch)
			lastIsUnderscore = false
		} else if isIntDigit(ch, 10) {
			panic(errIntBadDigit[base])
		} else if ch == '_' {
			if lastIsUnderscore {
				panic(errIntBadUnderscore)
			}
			lastIsUnderscore = true
		} else {
			break
		}
	}
	if lastIsUnderscore {
		panic(errIntBadUnderscore)
	}
	str := b.String()
	checkValidInteger(str, base)
	s.Tok = token.INT
	s.Lit = str
}

func checkValidInteger(str string, base intbase) {
	if len(str) > 2 || str[0] != '0' {
		return
	}
	last := rune(str[1])
	if isIntDigit(last, base) {
		return
	}
	panic(errIntHasNodigits[base])
}

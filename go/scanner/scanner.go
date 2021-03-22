/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * scanner.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package scanner

import (
	"io"
	"strings"

	"github.com/cosmos72/onejit/go/token"
)

type Scanner struct {
	item
	utf8Reader
	builder strings.Builder
}

type item struct {
	tok token.Token
	lit string
}

func NewScanner() *Scanner {
	return NewScannerFrom(nil)
}

func NewScannerFrom(src io.Reader) *Scanner {
	s := &Scanner{}
	s.Reset(src)
	return s
}

// clear internal buffers and accumulated errors, then change io.Reader
func (s *Scanner) Reset(src io.Reader) {
	s.item = item{}
	s.utf8Reader.reset(src)
	s.builder.Reset()
	if s.next() == runeBOM {
		s.next()
	}
}

func (s *Scanner) add() {
	s.addRune(s.ch)
}

func (s *Scanner) addRune(ch rune) {
	if ch != runeEOF {
		s.builder.WriteRune(ch)
	}
}

func (s *Scanner) addString(str string) {
	s.builder.WriteString(str)
}

func (s *Scanner) getString() string {
	return s.builder.String()
}

func (s *Scanner) clearString() {
	s.builder.Reset()
}

func (s *Scanner) setResult(tok token.Token) {
	s.tok = tok
	s.lit = s.getString()
}

func (s *Scanner) Errors() []error {
	return s.utf8Reader.err
}

func (s *Scanner) Scan() (token.Token, string) {
	s.clearString()

	ch := s.ch
	for ch != runeEOF && isSpace(ch) {
		ch = s.next()
	}
	if ch == runeEOF {
		s.tok = token.EOF
	} else if isDecimalDigit(ch) {
		s.scanNumber()
	} else if ch == '"' {
		s.scanString()
	} else if ch == '\'' {
		s.scanRune()
	} else if ch == '`' {
		s.scanRawString()
	} else if ch == '.' {
		s.scanDot()
	} else if isLetter(ch) {
		s.scanIdentifier()
	} else if isOperator(ch) {
		s.scanOperatorOrComment()
	} else {
		s.invalid(errInvalidCharacter)
	}
	return s.tok, s.lit
}

func (s *Scanner) scanIdentifier() {
	s.add()
	ch := s.next()
	for isLetter(ch) || isDigit(ch) {
		s.addRune(ch)
		ch = s.next()
	}
	str := s.getString()
	tok := token.Lookup(str)
	if tok != token.IDENT {
		s.clearString()
	}
	s.setResult(tok)
}

const unknownToken = token.ILLEGAL

func (s *Scanner) scanOperatorOrComment() {

	switch s.ch {
	case '/':
		s.scanSlash()
	default:
		s.scanOperator(unknownToken)
	}
}

func (s *Scanner) scanOperator(op token.Token) {
	next := unknownToken
	ch := s.ch
	for ch != runeEOF {
		s.addRune(ch)
		next = token.LookupOperator(s.getString())
		if next == unknownToken {
			break
		}
		op = next
		ch = s.next()
	}
	if op == unknownToken {
		s.error(errInvalidCharacter)
		s.next()
	} else {
		s.clearString()
	}
	s.setResult(op)
}

// scan characters after '.'
func (s *Scanner) scanDot() {
	// special case:
	// operator ... is the only one where removing last character
	// does not create another valid operator
	ch := s.next()
	if ch != '.' {
		if isDecimalDigit(ch) {
			// found ".digit" i.e. fraction-only floating point number
			s.scanIntOrFloat(base10, '.')
			return
		} else {
			// found ".nondigit"
			s.tok = token.PERIOD
			s.lit = ""
			return
		}
	}
	// found "..""
	ch = s.next()
	if ch != '.' {
		// found "..nondot"
		s.addString("..")
		if ch == runeEOF {
			s.invalid(errUnexpectedEOF)
		} else {
			s.invalid(errInvalidOperator)
		}
		return
	}
	// found "...""
	s.next()
	s.tok = token.ELLIPSIS
	s.lit = ""
}

// scan characters after '/'
func (s *Scanner) scanSlash() {
	ch := s.next()
	switch ch {
	case '/':
		s.scanCommentLine()
	case '*':
		s.scanCommentBlock()
	default:
		// found "/something"
		s.addRune('/')
		s.scanOperator(token.QUO)
	}
}

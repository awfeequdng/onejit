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
	Item
	utf8Reader
	builder strings.Builder
}

func NewScanner() *Scanner {
	s := &Scanner{}
	s.utf8Reader.init()
	return s
}

func NewScannerFrom(src io.Reader) *Scanner {
	s := NewScanner()
	s.SetReader(src)
	return s
}

// clear internal buffers
func (s *Scanner) Reset() {
	s.Item = Item{}
	s.utf8Reader.reset()
	s.builder.Reset()
}

func (s *Scanner) SetReader(src io.Reader) {
	s.utf8Reader.src = src
}

func (s *Scanner) Errors() []error {
	return s.utf8Reader.err
}

func (s *Scanner) Scan() {
	ch := s.ch
	if ch == runeBOF {
		ch = s.next()
	}
	if isDecimalDigit(ch) {
		s.scanNumber()
	} else if ch == '"' {
		s.scanString()
	} else if ch == '\'' {
		s.scanRune()
	} else if ch == '`' {
		s.scanRawString()
	}
}

func (s *Scanner) error(err error) bool {
	s.err = append(s.err, err)
	return false
}

func (s *Scanner) invalid(err error) bool {
	s.Tok = token.ILLEGAL
	if !isSpace(s.ch) {
		s.builder.WriteRune(s.ch)
	}
	s.Lit = s.builder.String()
	s.next()
	return s.error(err)
}

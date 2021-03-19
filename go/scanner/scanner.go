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

func (s *Scanner) SetReader(src io.Reader) {
	s.utf8Reader.src = src
	s.next()
}

func (s *Scanner) Scan() token.Token {
	s.number()
	return s.Tok
}

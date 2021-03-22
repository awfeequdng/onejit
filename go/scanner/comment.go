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

import "github.com/cosmos72/onejit/go/token"

// scan characters after "//"
func (s *Scanner) scanCommentLine() {
	s.addString("//")
	for {
		ch := s.next()
		if ch == runeEOF || ch == '\n' {
			break
		}
		// cleanup final "\r\n" on Windows
		if ch != '\r' {
			s.addRune(ch)
		}
	}
	s.setResult(token.COMMENT)
}

// scan characters after "/*"
func (s *Scanner) scanCommentBlock() {
	s.addString("/*")
	star := false
	for {
		ch := s.next()
		if ch == runeEOF {
			break
		}
		s.addRune(ch)
		if star && ch == '/' {
			break
		} else {
			star = ch == '*'
		}
	}
	s.next()
	s.setResult(token.COMMENT)
}

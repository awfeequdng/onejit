/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * identifier.go
 *
 *  Created on: Mar 21, 2021
 *      Author: Massimiliano Ghilardi
 */

package scanner

import (
	"github.com/cosmos72/onejit/go/token"
)

func (s *Scanner) scanIdentifier() {
	b := &s.builder
	b.Reset()
	b.WriteRune(s.ch)
	ch := s.next()
	for isLetter(ch) || isDigit(ch) {
		b.WriteRune(ch)
		ch = s.next()
	}
	str := b.String()
	tok := token.Lookup(str)
	if tok != token.IDENT {
		str = ""
	}
	s.Tok = tok
	s.Lit = str
}

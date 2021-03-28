/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * token.go
 *
 *  Created on: Mar 25, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/token"
)

func isAssign(tok token.Token) bool {
	switch tok {
	case token.ASSIGN, token.DEFINE:
		return true
	default:
		return false
	}
}

func isDecl(tok token.Token) bool {
	switch tok {
	case token.CONST, token.FUNC, token.TYPE, token.VAR:
		return true
	default:
		return false
	}
}

func isLeave(tok token.Token) bool {
	switch tok {
	case token.ILLEGAL, token.EOF, token.RPAREN, token.RBRACK, token.RBRACE:
		return true
	default:
		return false
	}
}

// return true if tok introduces a simple statement instead of an expression
func isSimpleStmt(tok token.Token) bool {
	switch tok {
	case token.INC, token.DEC, token.ASSIGN, token.DEFINE, token.ARROW:
		return true
	default:
		return false
	}
}

func isUnary(tok token.Token) bool {
	switch tok {
	case token.ADD, token.SUB, token.MUL, token.AND, token.XOR, token.ARROW, token.NOT:
		return true
	default:
		return false
	}
}

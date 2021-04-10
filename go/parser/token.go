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
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/token"
)

func isAssign(tok token.Token) bool {
	switch tok {
	case token.ASSIGN, token.DEFINE, token.ADD_ASSIGN, token.SUB_ASSIGN, token.MUL_ASSIGN,
		token.QUO_ASSIGN, token.REM_ASSIGN, token.AND_ASSIGN, token.OR_ASSIGN, token.XOR_ASSIGN,
		token.SHL_ASSIGN, token.SHR_ASSIGN, token.AND_NOT_ASSIGN:
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

/*
func isEnter(tok token.Token) bool {
	switch tok {
	case token.LPAREN, token.LBRACK, token.LBRACE:
		return true
	default:
		return false
	}
}
*/

func isLeave(tok token.Token) bool {
	switch tok {
	case token.ILLEGAL, token.EOF, token.RPAREN, token.RBRACK, token.RBRACE:
		return true
	default:
		return false
	}
}

// return true if node is a simple statement rather than an expression
func isSimpleStmt(node ast.Node) bool {
	if node == nil {
		return false
	}
	switch tok := node.Op(); tok {
	case token.INC, token.DEC:
		return true
	case token.ARROW:
		// (<- x) is a receive expression
		// (<- x y) is a send statement
		return node.Len() == 2
	default:
		return isAssign(tok)
	}
}

// return true if tok may introduce a type
func isTypeStart(tok token.Token) bool {
	switch tok {
	case token.IDENT, token.MUL, token.ARROW, token.LPAREN, token.LBRACK,
		token.CHAN, token.FUNC, token.INTERFACE, token.MAP, token.STRUCT:
		return true
	default:
		return false
	}
}

// return true if tok may introduce a type or '... type'
func isTypeStartOrEllipsis(tok token.Token) bool {
	return tok == token.ELLIPSIS || isTypeStart(tok)
}

func isUnary(tok token.Token) bool {
	switch tok {
	case token.ADD, token.SUB, token.MUL, token.AND, token.XOR, token.ARROW, token.NOT:
		return true
	default:
		return false
	}
}

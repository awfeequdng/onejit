/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * error.go
 *
 *  Created on: Mar 24, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"fmt"

	"github.com/cosmos72/onejit/go/scanner"
	"github.com/cosmos72/onejit/go/token"
)

type errText string

const (
	errExpectingAssignDefineOrComma = ":= or = or comma"
	errExpectingCaseOrDefault       = "case or default or }"
	errExpectingConstVarFuncOrType  = "'const' 'var' 'func' or 'type'"
	errExpectingChan                = "chan"
	errExpectingDecl                = "declaration"
	errExpectingExpr                = "expression"
	errExpectingExprOrType          = "expression or type"
	errExpectingIdent               = "identifier"
	errExpectingIdentOrLparen       = "identifier or ("
	errExpectingString              = "string"
	errExpectingType                = "type"
	errDuplicatePackage             = errText("duplicate package declaration")
	errEmptyTypeParams              = errText("empty type parameter list")
	errExpectedOneExpr              = errText("expected 1 expression")
	errInvalidTypeSwitch            = errText("use of .(type) outside type switch")
	errParamsNamedUnnamed           = errText("syntax error: mixed named and unnamed function parameters")
	errParamNonFinalEllipsis        = errText("syntax error: cannot use ... with non-final parameter")
	errSelectCaseNotSendOrRecv      = errText("select case must be send, receive or assign recv")
	errTypeAlias                    = errText("type aliases are disabled, they require parser flag TypeAlias")
	errGenerics                     = errText("generics are disabled, they require parser flag Generics")
)

func (p *Parser) makeErrText(suffix string) errText {
	return errText("syntax error: unexpected " + p.tok().String() + ", expecting " + suffix)
}

func (p *Parser) error(pos token.Pos, msg interface{}) *scanner.Error {
	var text errText
	switch msg := msg.(type) {
	case errText:
		text = msg
	case string:
		text = p.makeErrText(msg)
	default:
		text = p.makeErrText(fmt.Sprint(msg))
	}
	return p.scanner.Error(pos, string(text))
}

func (p *Parser) Errors() *[]*scanner.Error {
	return p.scanner.Errors()
}

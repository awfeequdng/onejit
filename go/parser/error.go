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
	"github.com/cosmos72/onejit/go/scanner"
)

const (
	errExpectingConstVarFuncOrType = "'const' 'var' 'func' or 'type'"
	errExpectingChan               = "chan"
	errExpectingIdent              = "identifier"
	errExpectingString             = "string"
	errExpectingType               = "type"
)

func (p *Parser) error(msg string) {
	p.err = append(p.err, &scanner.Error{
		Pos: p.scanner.Position(p.pos()),
		Msg: "syntax error: unexpected " + p.tok().String() + ", expecting " + msg,
	})
}

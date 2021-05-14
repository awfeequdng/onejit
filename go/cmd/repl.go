/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * main.go
 *
 *  Created on: Apr 08, 2021
 *      Author: Massimiliano Ghilardi
 */

package cmd

import (
	"github.com/cosmos72/onejit/go/config"
	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/parser"
	"github.com/cosmos72/onejit/go/token"
)

func Repl(in io.Reader, out io.StringWriter) {
	var p parser.Parser
	out.WriteString("go> ")
	p.Init(token.NewFile("repl.go", 0), in, parser.ParseAll, config.AllFeatures)
	for {
		node := p.Parse()
		if node != nil {
			out.WriteString(node.String())
			out.WriteString("\n")
			if node.Op() == token.EOF {
				break
			}
		}
		out.WriteString("go> ")
	}
}

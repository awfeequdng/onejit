/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * file.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package parser

import (
	"github.com/cosmos72/onejit/go/ast"
	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/token"
)

// parse all files in a directory
func (p *Parser) ParseDir(fset *token.FileSet, opener func() (filename string, src io.ReadCloser)) []*ast.File {
	var dir []*ast.File
	for {
		file := p.ParseDirFile(fset, opener)
		if file == nil {
			break
		}
		dir = append(dir, file)
	}
	return dir
}

// parse a single file in a directory
func (p *Parser) ParseDirFile(fset *token.FileSet, opener func() (filename string, src io.ReadCloser)) *ast.File {
	filename, src := opener()
	if src == nil {
		return nil
	}
	defer src.Close()
	p.Init(fset.AddFile(filename), src, p.Mode)
	return p.ParseFile()
}

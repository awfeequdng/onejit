/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * field.go
 *
 *  Created on: Apr 03, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/strings"
)

type CompleteField struct {
	Type     *Complete
	Name     string
	PkgPath  string
	Tag      string
	Offset   uint64
	Index    int
	Embedded bool
}

type Field struct {
	Type     Type
	Name     string
	PkgPath  string
	Tag      string
	Offset   uint64
	Index    int
	Embedded bool
}

func (f *Field) WriteTo(dst io.StringWriter, flag verbose) {
	if !f.Embedded {
		strings.WriteQualifiedName(dst, f.Name, f.PkgPath, flag)
		dst.WriteString(" ")
	}
	f.Type.WriteTo(dst, flag)
}

func (f *Field) hash(h hash) hash {
	if f == nil || f.Type == nil {
		return unknownHash
	}
	typhash := f.Type.common().hash
	if typhash == unknownHash {
		return unknownHash
	}
	// ignore Field Tag when computing hash
	return h.Hash(typhash).String(f.Name).String(f.PkgPath).Bool(f.Embedded)
}

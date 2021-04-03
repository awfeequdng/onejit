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

import "strings"

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

func (f *Field) writeTo(b *strings.Builder, flag verbose) {
	if !f.Embedded {
		writeQualifiedName(b, f.Name, f.PkgPath, flag)
		b.WriteByte(' ')
	}
	f.Type.writeTo(b, flag)
}

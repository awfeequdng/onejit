/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * named.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import "github.com/cosmos72/onejit/go/strings"

// returned by Complete.Method(int)
type CompleteMethod struct {
	Type    *Complete
	Name    string
	PkgPath string
	Index   int
}

// returned by Named.Method(int)
type Method struct {
	Type    Type
	Name    string
	PkgPath string
	Index   int
}

func (m *Method) String() string {
	var b builder
	m.writeTo(&b, fullPkgPath)
	return b.String()
}

func (m *Method) writeTo(b *builder, flag verbose) {
	strings.WriteQualifiedName(b, m.Name, m.PkgPath, flag)

	sig := m.Type.(*Func)
	// omits "func" prefix
	writeFuncTo(b, sig.in(), sig.out(), sig.IsVariadic(), flag)
}

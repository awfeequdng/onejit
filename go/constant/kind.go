/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * kind.go
 *
 *  Created on: Apr 12, 2021
 *      Author: Massimiliano Ghilardi
 */

package constant

import "github.com/cosmos72/onejit/go/token"

// alias for token.Kind
type Kind = token.Kind

const (
	Invalid = token.Invalid

	// kinds of untyped values
	Bool    = token.UntypedBool
	Int     = token.UntypedInt
	Rune    = token.UntypedRune
	Float   = token.UntypedFloat
	Complex = token.UntypedComplex
	String  = token.UntypedString
	Nil     = token.UntypedNil
)

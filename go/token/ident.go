/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * ident.go
 *
 *  Created on: Apr 10, 2021
 *      Author: Massimiliano Ghilardi
 */

package token

import (
	"unicode"
	"unicode/utf8"
)

func IsExported(name string) bool {
	if len(name) == 0 {
		return false
	}
	if b := name[0]; b < utf8.RuneSelf {
		return b >= 'A' && b <= 'Z'
	}

	ch, _ := utf8.DecodeRuneInString(name)
	return unicode.IsUpper(ch)
}

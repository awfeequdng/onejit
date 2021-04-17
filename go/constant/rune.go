/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * rune.go
 *
 *  Created on: Apr 12, 2021
 *      Author: Massimiliano Ghilardi
 */

package constant

import (
	"go/constant"
	"unicode/utf8"
)

func untypedRune2string(c constant.Value) string {
	n, exact := constant.Uint64Val(c)
	if exact && n <= utf8.MaxRune {
		if r := rune(n); utf8.ValidRune(r) {
			if r == '\\' {
				return `'\\'`
			} else if r == '\'' {
				return `'\''`
			} else if r >= ' ' && r <= '~' {
				return string([]byte{'\'', byte(r), '\''})
			} else if r <= 0xffff {
				return quoteRune('u', r, 4)
			} else {
				return quoteRune('U', r, 8)
			}
		}
	}
	return c.String()
}

func quoteRune(prefix byte, r rune, len int) string {
	buf := []byte{'\'', '\\', prefix, 0, 0, 0, 0, 0, 0, 0, 0, '\''}
	pos := 3
	number := uint32(r)
	for shift := (len - 1) * 4; shift >= 0; shift -= 4 {
		halfbyte := 0x0f & uint8(number>>uint8(shift))
		buf[pos] = "0123456789abcdef"[halfbyte]
		pos++
	}
	buf[pos] = '\''
	return string(buf[:pos+1])
}

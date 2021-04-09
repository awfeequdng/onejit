/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * escape.go
 *
 *  Created on: Apr 02, 2021
 *      Author: Massimiliano Ghilardi
 */

package strings

// convert ASCII uppercase letters to lowercase
func ToLower(str string) string {
	n := len(str)
	isTrivial := true
	for i := 0; i < n; i++ {
		ch := str[i]
		if ch >= 'A' && ch <= 'Z' {
			isTrivial = false
			break
		}
	}
	if isTrivial {
		return str
	}
	buf := make([]byte, n)
	for i := 0; i < n; i++ {
		ch := str[i]
		if ch >= 'A' && ch <= 'Z' {
			ch += 'a' - 'A'
		}
		buf[i] = ch
	}
	return string(buf)
}

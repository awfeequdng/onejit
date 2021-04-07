/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * string.go
 *
 *  Created on: Apr 02, 2021
 *      Author: Massimiliano Ghilardi
 */

package strings

func IntToString(n int) string {
	return Int64ToString(int64(n))
}

func UintToString(n uint) string {
	return Uint64ToString(uint64(n))
}

func Int64ToString(i int64) string {
	if i == 0 {
		return "0"
	}
	var b [20]byte
	sign := i < 0
	n := uint64(i)
	if sign {
		// i = -i would fail for most negative int64
		n = -n
	}
	pos := len(b)
	for n != 0 {
		pos--
		b[pos] = '0' + byte(n%10)
		n /= 10
	}
	if sign {
		pos--
		b[pos] = '-'
	}
	return string(b[pos:])
}

func Uint64ToString(n uint64) string {
	if n == 0 {
		return "0"
	}
	var b [20]byte
	pos := len(b)
	for n != 0 {
		pos--
		b[pos] = '0' + byte(n%10)
		n /= 10
	}
	return string(b[pos:])
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * unicode.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package scanner

import (
	"unicode"
)

type intbase int

const (
	baseAny intbase = iota
	base2
	base8
	base16
	base10
)

const runeBOF rune = -2     // beginning of file i.e. no rune was read yet
const runeBOM rune = 0xfeff // byte order marker
const runeEOF rune = -1     // end of file

func isSpace(ch rune) bool {
	switch ch {
	case '\t', '\r', '\n', ' ', runeEOF:
		return true
	default:
		return false
	}
}

func isOperator(ch rune) bool {
	return (ch >= '!' && ch <= '/') ||
		(ch >= ':' && ch <= '@') ||
		(ch >= '[' && ch <= '^') || ch == '`' ||
		(ch >= '{' && ch <= '~')
}

func isLetter(ch rune) bool {
	return unicode.IsLetter(ch)
}

func isDigit(ch rune) bool {
	return unicode.IsDigit(ch)
}

func isBinaryDigit(ch rune) bool {
	return ch >= '0' && ch <= '1'
}

func isOctalDigit(ch rune) bool {
	return ch >= '0' && ch <= '7'
}

func isDecimalDigit(ch rune) bool {
	return ch >= '0' && ch <= '9'
}

func isIntDigit(ch rune, base intbase) bool {
	switch base {
	case base2:
		return isBinaryDigit(ch)
	case base8:
		return isOctalDigit(ch)
	case base16:
		return isHexDigit(ch)
	case base10:
		fallthrough
	default:
		return isDecimalDigit(ch)
	}
}

func isHexDigit(ch rune) bool {
	return (ch >= '0' && ch <= '9') ||
		(ch >= 'A' && ch <= 'F') ||
		(ch >= 'a' && ch <= 'f')
}

func hexDigitToInt(ch rune) int {
	if ch >= '0' && ch <= '9' {
		return int(ch - '0')
	} else if ch >= 'A' && ch <= 'F' {
		return int(ch-'A') + 10
	} else if ch >= 'a' && ch <= 'f' {
		return int(ch-'a') + 10
	}
	return 0
}

/*
func trim(str string) string {
	start, end := 0, len(str)
	for ; start < end; start++ {
		if !isSpace(rune(str[start])) {
			break
		}
	}
	for ; start < end; end-- {
		if !isSpace(rune(str[end-1])) {
			break
		}
	}
	return str[start:end]
}
*/

func lower(ch byte) byte {
	if ch >= 'A' && ch <= 'Z' {
		ch += 'a' - 'A'
	}
	return ch
}

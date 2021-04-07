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

// interpret \ inside a Go literal string
func Unescape(str string) string {
	pos := IndexByte(str, '\\')
	if pos < 0 {
		return str
	}
	var b Builder
	b.Grow(len(str))
	for pos >= 0 {
		b.WriteString(str[:pos])
		pos++ // skip \
		if len(str) <= pos {
			break
		}
		curr := str[pos]
		str = str[pos:]
		skip := 1
		unicode := int32(-1)
		ch := int16(-1)
		switch curr {
		case 'a':
			ch = '\a'
		case 'b':
			ch = '\b'
		case 'f':
			ch = '\f'
		case 'n':
			ch = '\n'
		case 'r':
			ch = '\r'
		case 't':
			ch = '\t'
		case 'v':
			ch = '\v'
		case '\\', '\'', '"':
			ch = int16(curr)
		case 'U':
			unicode, str = hexDigitsToInt(str, 8)
			skip = 0
		case 'u':
			unicode, str = hexDigitsToInt(str, 4)
			skip = 0
		case 'x':
			var hex int32
			hex, str = hexDigitsToInt(str, 2)
			ch = int16(hex)
			skip = 0
		default:
			if curr >= '0' && curr <= '7' {
				var octal int32
				octal, str = octalDigitsToInt(str, 3, 255)
				ch = int16(octal)
				skip = 0
			}
		}
		str = str[skip:]
		if unicode >= 0 {
			b.WriteRune(unicode)
		} else if ch >= 0 {
			b.WriteByte(uint8(ch))
		}
		pos = IndexByte(str, '\\')
	}
	return b.String()
}

func hexDigitsToInt(str string, n int) (int32, string) {
	avail := len(str)
	if avail < n {
		return -1, ""
	}
	var hex int32
	for i := 0; i < n; i++ {
		hex = (hex << 4) | hexDigitToInt(str[i])
	}
	return hex, str[n:]
}

func octalDigitsToInt(str string, n int, max int32) (int32, string) {
	avail := len(str)
	if avail < n {
		return -1, ""
	}
	var oct int32
	for i := 0; i < n; i++ {
		oct = (oct << 3) | octalDigitToInt(str[i])
	}
	if oct > max {
		oct = -1
	}
	return oct, str[n:]
}

func hexDigitToInt(ch byte) int32 {
	if ch >= '0' && ch <= '9' {
		return int32(ch - '0')
	} else if ch >= 'A' && ch <= 'F' {
		return int32(ch-'A') + 10
	} else if ch >= 'a' && ch <= 'f' {
		return int32(ch-'a') + 10
	} else {
		return -1
	}
}

func octalDigitToInt(ch byte) int32 {
	if ch >= '0' && ch <= '7' {
		return int32(ch - '0')
	} else {
		return -1
	}
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * builder.go
 *
 *  Created on: Apr 07, 2021
 *      Author: Massimiliano Ghilardi
 */

package strings

import "unicode/utf8"

// reduced version of strings.Builder
type Builder struct {
	buf []byte
}

func (b *Builder) Grow(n int) {
	oldcap, oldlen := cap(b.buf), len(b.buf)
	if oldcap-oldlen < n {
		newcap := oldcap * 2
		if newcap-oldlen < n {
			newcap = oldlen + n
		}
		buf := make([]byte, oldlen, newcap)
		copy(buf, b.buf)
		b.buf = buf
	}

}

func (b *Builder) Len() int {
	return len(b.buf)
}

func (b *Builder) Reset() {
	b.buf = nil
}

func (b *Builder) WriteByte(c byte) error {
	b.buf = append(b.buf, c)
	return nil
}

func (b *Builder) WriteRune(r rune) (n int, err error) {
	if r < utf8.RuneSelf {
		b.buf = append(b.buf, byte(r))
		return 1, nil
	}
	oldn, max := len(b.buf), utf8.UTFMax
	b.Grow(max)
	n = utf8.EncodeRune(b.buf[oldn:oldn+max], r)
	b.buf = b.buf[:oldn+n]
	return n, nil
}

func (b *Builder) WriteString(str string) (n int, err error) {
	b.buf = append(b.buf, str...)
	return len(str), nil
}

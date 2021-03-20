/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * utf8reader.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package scanner

import (
	"errors"
	"io"
	"unicode/utf8"
)

var ErrInvalidUtf8 = errors.New("invalid UTF-8 sequence")

type utf8Reader struct {
	ch    rune // next rune
	buf   []byte
	start int // offset into buf
	src   io.Reader
}

func (u *utf8Reader) init() {
	u.ch = runeBOF
	u.buf = make([]byte, 0, 65536)
	u.start = 0
	u.src = alwaysEof
}

// clear internal buffers
func (u *utf8Reader) reset() {
	u.ch = runeBOF
	u.buf = u.buf[0:0:cap(u.buf)]
	u.start = 0
}

func (u *utf8Reader) empty() bool {
	return u.start >= len(u.buf)
}

// read next rune and return it. also save it in u.ch
func (u *utf8Reader) next() rune {
	if u.empty() {
		u.refill()
		if u.empty() {
			u.ch = runeEOF
			return runeEOF
		}
	}
	ch, size := utf8.DecodeRune(u.buf[u.start:])
	if ch == utf8.RuneError && size <= 1 {
		panic(ErrInvalidUtf8)
	}
	u.start += size
	u.ch = ch
	return ch
}

func (u *utf8Reader) refill() {
	if u.start >= cap(u.buf)/2 {
		u.shift()
	}
	buf := u.buf
	end, max := len(buf), cap(buf)
	if end >= max {
		return
	}
	got, err := u.src.Read(buf[end:max])
	if got > 0 {
		u.buf = buf[0 : end+got : max]
	} else if err != nil && !errors.Is(io.EOF, err) {
		panic(err)
	}
	// else EOF
}

func (u *utf8Reader) shift() {
	buf := u.buf
	start, end, max := u.start, len(buf), cap(buf)
	dst := buf[0:0:max]
	if start < end {
		dst = buf[0 : end-start : max]
		copy(dst, buf[start:end])
	}
	u.start = 0
	u.buf = buf[0 : end-start : max]
}

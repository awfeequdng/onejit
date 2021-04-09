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
	"unicode/utf8"

	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/token"
)

type utf8Reader struct {
	ch rune // next rune. may also be runeBOF, runeNONE or runeEOF
	// if unread != runeNONE, will be consumed and returned
	// by next() instead of reading from buf
	unread rune
	buf    []byte
	start  int // endpos into buf
	src    io.Reader
	file   *token.File
	pos    int
	endpos int
	errors []*Error
}

// (re)initialize utf8Reader
// does NOT clear accumulated errors
func (u *utf8Reader) init(file *token.File, src io.Reader) {
	u.ch = runeBOF
	u.unread = runeNONE
	size := 65536
	if cap(u.buf) != size {
		u.buf = make([]byte, 0, size)
	} else {
		u.buf = u.buf[0:0:cap(u.buf)]
	}
	u.start = 0
	if src == nil {
		src = alwaysEof
	} else {
		u.src = src
	}
	u.file = file
	pos := file.Base()
	u.pos = pos
	u.endpos = pos
}

func (u *utf8Reader) empty() bool {
	return u.start >= len(u.buf)
}

// read next rune and return it. also save it in u.ch
func (u *utf8Reader) next() rune {
	if u.unread != runeNONE {
		if u.unread == runeTOO_MANY_ERRORS {
			u.ch = runeEOF
		} else {
			u.ch = u.unread
			u.unread = runeNONE
		}
		return u.ch
	}

	for {
		buf := u.buf[u.start:]
		if !utf8.FullRune(buf) {
			u.refill()
			if u.empty() {
				return u.foundEOF()
			}
			buf = u.buf[u.start:]
		}
		ch, size := utf8.DecodeRune(buf)
		if ch == utf8.RuneError && size <= 1 {
			u.foundInvalidUtf8()
			continue
		}
		u.start += size
		u.endpos += size
		if ch == '\n' {
			u.file.AddLine(u.endpos)
		}
		u.ch = ch
		break
	}
	return u.ch
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
		u.error(err.Error())
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

func (u *utf8Reader) foundInvalidUtf8() {
	u.error(errInvalidUtf8)
	u.start++
	u.endpos++
}

func (u *utf8Reader) foundEOF() rune {
	f := u.file
	f.SetSize(u.endpos - f.Base())
	u.ch = runeEOF
	return runeEOF
}

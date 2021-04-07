/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * reader.go
 *
 *  Created on: Apr 07, 2021
 *      Author: Massimiliano Ghilardi
 */

package strings

import (
	"github.com/cosmos72/onejit/go/io"
)

// Reduced version of strings.Reader, implements only io.Reader
// by reading from a string.
// The zero value is a Reader of an empty string.
type Reader struct {
	str string
}

func (r *Reader) Read(p []byte) (n int, err error) {
	str := r.str
	n = len(str)
	if n == 0 {
		return 0, io.EOF
	}
	copy(p, str)
	if n > len(p) {
		n = len(p)
	}
	r.str = str[n:]
	return n, nil
}

func (r *Reader) Reset(str string) {
	r.str = str
}

// return a new Reader reading from specified string str.
func NewReader(str string) *Reader {
	return &Reader{str}
}

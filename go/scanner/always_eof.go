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
	"io"
)

type readerAlwaysEof struct{}

func (r readerAlwaysEof) Read(dst []byte) (int, error) {
	return 0, io.EOF
}

var alwaysEof io.Reader = readerAlwaysEof{}

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

package io

// equivalent to io.Reader
type Reader interface {
	Read(p []byte) (n int, err error)
}

// equivalent to io.Writer
type Writer interface {
	Write(p []byte) (n int, err error)
}

// +build gc,!safe

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * builder_unsafe.go
 *
 *  Created on: Apr 07, 2021
 *      Author: Massimiliano Ghilardi
 */

package strings

import "unsafe"

func (b *Builder) String() string {
	// more efficient, but requires unsafe and breaks horribly if non-zero Builder is copied
	return *(*string)(unsafe.Pointer(&b.buf))
}

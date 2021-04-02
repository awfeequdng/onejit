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

package types

import "strings"

func basename(path string) string {
	slash := strings.LastIndexByte(path, '/')
	return path[slash+1:]
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * os.go
 *
 *  Created on: Apr 06, 2021
 *      Author: Massimiliano Ghilardi
 */

package arch

type Os uint16

const (
	Linux Os = iota
	Windows
	OsOther // Mac OS X, *BSD...
	os_end
)

func (os Os) String() string {
	var str string
	switch os {
	case Linux:
		str = "linux"
	case Windows:
		str = "windows"
	case OsOther:
		str = "other"
	default:
		str = "unknown"
	}
	return str
}

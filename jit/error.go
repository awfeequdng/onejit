/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * error.go
 *
 *  Created on Dec 28, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

import (
	"github.com/cosmos72/gomacrojit/jit/internal"
)

func Debugf(format string, args ...interface{}) {
	internal.Debugf(format, args...)
}

func Warnf(format string, args ...interface{}) {
	internal.Warnf(format, args...)
}

func Errorf(format string, args ...interface{}) {
	internal.Errorf(format, args...)
}

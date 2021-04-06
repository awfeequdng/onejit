// +build !386,!amd64,!arm,!arm64

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * arch_other.go
 *
 *  Created on: Apr 06, 2021
 *      Author: Massimiliano Ghilardi
 */

package arch

// autodetect if arch is 32 or 64 bits
const ArchAuto = ArchOther32bit + Arch(^uint(0)>>63)

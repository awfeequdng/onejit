/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * config.go
 *
 *  Created on: Apr 06, 2021
 *      Author: Massimiliano Ghilardi
 */

package arch

var (
	targetOs   Os   = OsAuto
	targetArch Arch = ArchAuto
)

// return the current target Operating System
// and the current target architecture,
// i.e. for which OS and arch code should be compiled.
func Target() (Os, Arch) {
	return targetOs, targetArch
}

// return the current target Operating System
func TargetOs() Os {
	return targetOs
}

// return the current target Operating System
func TargetArch() Arch {
	return targetArch
}

/**
 * set the current target Operating System
 * and set current target architecture,
 * i.e. for which OS and arch code should be compiled.
 *
 * To autodetect operating system, specify OsAuto.
 * To autodetect architecture, specify ArchAuto.
 *
 * Results returned by types.BasicType() and types.BasicTypes() depend
 * on the arguments passed to this function.
 */
func SetTarget(os Os, arch Arch) {
	if os >= os_end {
		panic("SetConfig: unsupported OS")
	} else if arch >= arch_end {
		panic("SetConfig: unsupported Arch")
	}
	targetOs = os
	targetArch = arch
}

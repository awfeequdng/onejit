/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * lang.go
 *
 *  Created on: May 13, 2021
 *      Author: Massimiliano Ghilardi
 */

package config

type Lang uint32

const (
	Go1_8  Lang = 0x00010008 // minimum supported Go language version
	Go1_9  Lang = 0x00010009 // also enable TypeAlias
	Go1_10 Lang = 0x0001000a
	Go1_11 Lang = 0x0001000b
	Go1_12 Lang = 0x0001000c
	Go1_13 Lang = 0x0001000d
	Go1_14 Lang = 0x0001000e
	Go1_15 Lang = 0x0001000f
	Go1_16 Lang = 0x00010010
	Go1_17 Lang = 0x00010011 // also enable ConvertSliceToArrayPtr
	go1_xy Lang = 0x00010fff // also enable Generics

	TypeAlias              = Go1_9  // enable type aliases, added in Go 1.9
	ConvertSliceToArrayPtr = Go1_17 // also enable conversion []T -> *[N]T, added in Go 1.17
	Generics               = go1_xy // also enable generics, added in Go 1.xy

	AllFeatures = ^Lang(0) // enable all Go language features
)

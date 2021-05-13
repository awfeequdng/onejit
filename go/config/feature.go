/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * feature.go
 *
 *  Created on: May 13, 2021
 *      Author: Massimiliano Ghilardi
 */

package config

type Feature uint64

const (
	TypeAlias              Feature = 1 << iota // enable type aliases, added in Go 1.9
	ConvertSliceToArrayPtr                     // enable conversion []T -> *[N]T, added in Go 1.17
	Generics                                   // enable generics, added in Go 1.xy

	Go1_8       = TypeAlias - 1          // enable all features <  TypeAlias
	Go1_9       = TypeAlias              // enable all features <= TypeAlias
	Go1_17      = ConvertSliceToArrayPtr // enable all features <= ConvertSliceToArrayPtr
	AllFeatures = ^Feature(0)            // enable all features

)

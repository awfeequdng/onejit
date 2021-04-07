/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * visit.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package testutil

import (
	"os"

	"github.com/cosmos72/onejit/go/sort"
)

type sortableInfo []os.FileInfo

func (s sortableInfo) Len() int {
	return len(s)
}

func (s sortableInfo) Less(i, j int) bool {
	return s[i].Name() < s[j].Name()
}

func (s sortableInfo) Swap(i, j int) {
	s[i], s[j] = s[j], s[i]
}

func sortInfo(info []os.FileInfo) {
	sort.Sort(sortableInfo(info))
}

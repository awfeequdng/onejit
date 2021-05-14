/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * tags.go
 *
 *  Created on: May 14, 2021
 *      Author: Massimiliano Ghilardi
 */

package build

import "sort"

type Tags struct {
	tags map[string]struct{}
}

func MakeTags(tags ...string) Tags {
	m := make(map[string]struct{}, len(tags))
	for _, tag := range tags {
		m[tag] = struct{}{}
	}
	return Tags{m}
}

func MakeTagsMap(tags map[string]struct{}) Tags {
	m := make(map[string]struct{}, len(tags))
	for tag := range tags {
		m[tag] = struct{}{}
	}
	return Tags{m}
}

func (ts Tags) Find(tag string) bool {
	_, ok := ts.tags[tag]
	return ok
}

func (ts Tags) List() []string {
	list := make([]string, len(ts.tags))
	i := 0
	for tag := range ts.tags {
		list[i] = tag
		i++
	}
	sort.Strings(list)
	return list
}

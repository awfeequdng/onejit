/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2019 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * cache.go
 *
 *  Created on Feb 24, 2019
 *      Author Massimiliano Ghilardi
 */

package common

// map[len][crc32c][array of]executable machine code
type Cache map[int]map[uint32][]MemArea

func (cache Cache) Lookup(area MemArea) MemArea {
	// output.Debugf("cache lookup: %#04x %v", area.Checksum(), area)
	m := cache[area.Size()]
	if m != nil {
		v := m[area.Checksum()]
		if v != nil {
			for _, other := range v {
				if area.Equal(other) {
					// output.Debugf("cache hit:    %#04x %v", other.Checksum(), other)
					return other
				}
			}
		}
	}
	var ret MemArea // exploit zero value
	return ret
}

func (cache Cache) Add(area MemArea) {
	size := area.Size()
	m := cache[size]
	if m == nil {
		m = make(map[uint32][]MemArea)
		cache[size] = m
	}
	hash := area.Checksum()
	m[hash] = append(m[hash], area)
	// output.Debugf("cache add:    %#04x %v", hash, area)
}

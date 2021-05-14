/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * hash.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type hash uint64

const (
	fnv_prime  hash = 0x00000100000001b3
	fnv_offset hash = 0xcbf29ce484222325
)

func hashInit() hash {
	return fnv_offset
}

func (h hash) Init() hash {
	return fnv_offset
}

func (h hash) Uint8(data uint8) hash {
	return (h ^ hash(data)) * fnv_prime
}

func (h hash) Uint16(data uint16) hash {
	for i := 0; i < 2; i++ {
		h = (h ^ hash(data&0xff)) * fnv_prime
		data >>= 8
	}
	return h
}

func (h hash) Uint32(data uint32) hash {
	for i := 0; i < 4; i++ {
		h = (h ^ hash(data&0xff)) * fnv_prime
		data >>= 8
	}
	return h
}

func (h hash) Uint64(data uint64) hash {
	for i := 0; i < 8; i++ {
		h = (h ^ hash(data&0xff)) * fnv_prime
		data >>= 8
	}
	return h
}

func (h hash) Int(data int) hash {
	return h.Uint64(uint64(data))
}

func (h hash) Hash(data hash) hash {
	return h.Uint64(uint64(data))
}

func (h hash) Bool(data bool) hash {
	var x uint8 = 0x4e
	if data {
		x = 0xc3
	}
	return h.Uint8(x)
}

func (h hash) String(data string) hash {
	n := len(data)
	h = h.Int(n)
	for i := 0; i < n; i++ {
		h = h.Uint8(data[i])
	}
	return h
}

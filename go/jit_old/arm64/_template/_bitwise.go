/*
 * onejit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020 Massimiliano Ghilardi
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
 * arith.go
 *
 *  Created on May 27, 2018
 *      Author Massimiliano Ghilardi
 */

package arm64

//go:nosplit
func and_1(z int64) int64 {
	return z & 1
}

//go:nosplit
func and_2(z int64) int64 {
	return z & 2
}

//go:nosplit
func and_3(z int64) int64 {
	return z & 3
}

//go:nosplit
func and_255(z int64) int64 {
	return z & 255
}

//go:nosplit
func and_256(z int64) int64 {
	return z & 256
}

//go:nosplit
func and_2048(z int64) int64 {
	return z & 2048
}

//go:nosplit
func and_4095(z int64) int64 {
	return z & 4095
}

//go:nosplit
func or_1(z int64) int64 {
	return z | 1
}

//go:nosplit
func or_2(z int64) int64 {
	return z | 2
}

//go:nosplit
func or_3(z int64) int64 {
	return z | 3
}

//go:nosplit
func or_255(z int64) int64 {
	return z | 255
}

//go:nosplit
func or_256(z int64) int64 {
	return z | 256
}

//go:nosplit
func or_2048(z int64) int64 {
	return z | 2048
}

//go:nosplit
func or_4095(z int64) int64 {
	return z | 4095
}

//go:nosplit
func xor_1(z int64) int64 {
	return z ^ 1
}

//go:nosplit
func xor_2(z int64) int64 {
	return z ^ 2
}

//go:nosplit
func xor_3(z int64) int64 {
	return z ^ 3
}

//go:nosplit
func xor_255(z int64) int64 {
	return z ^ 255
}

//go:nosplit
func xor_256(z int64) int64 {
	return z ^ 256
}

//go:nosplit
func xor_2048(z int64) int64 {
	return z ^ 2048
}

//go:nosplit
func xor_4095(z int64) int64 {
	return z ^ 4095
}

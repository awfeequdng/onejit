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
 * zero.go
 *
 *  Created on Feb 07, 2019
 *      Author Massimiliano Ghilardi
 */

package main

// go:nosplit
func Zero0(env *Env) {
	env.IP++
}

// go:nosplit
func Zero8(env *Env) uint8 {
	return uint8(env.Ints[0])
}

// go:nosplit
func Zero16(env *Env) uint16 {
	return uint16(env.Ints[0])
}

// go:nosplit
func Zero32(env *Env) uint32 {
	return uint32(env.Ints[0])
}

// go:nosplit
func Zero64(env *Env) uint64 {
	return uint64(env.Ints[0])
}

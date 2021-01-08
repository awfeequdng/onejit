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
 * z_test.go
 *
 *  Created on Feb 10, 2019
 *      Author Massimiliano Ghilardi
 */

package common

import (
	"testing"
)

func EqUint8(t *testing.T, actual uint8, expected uint8) {
	if actual != expected {
		t.Errorf("expected %d,\tactual %d", expected, actual)
	}
}

func TestLog2(t *testing.T) {
	for shift := uint8(1); shift < 64; shift++ {
		n := uint64(1) << shift
		actual, _ := Log2Uint(n)
		EqUint8(t, actual, shift)
		actual, _ = Log2Uint(n - 1)
		EqUint8(t, actual, 0)
		actual, _ = Log2Uint(n + 1)
		EqUint8(t, actual, 0)
	}
}

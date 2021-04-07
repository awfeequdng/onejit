/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * search.go
 *
 *  Created on: Apr 07, 2021
 *      Author: Massimiliano Ghilardi
 */

package sort

// binary search: return the smallest i < n such that pred(i) is true.
// assumes that pred(x) is always false up to a certain x, then always true.
// if pred(n-1) is false, return n.
func Search(n int, pred func(int) bool) int {
	lo, hi := 0, n-1
	for lo < hi {
		mid := int(uint(lo+hi) >> 1)
		if pred(mid) {
			hi = mid
		} else {
			lo = mid + 1
		}
	}
	return lo
}

// binary search: return the smallest i < len(data) such that data[i] >= x.
// assumes that data[] is sorted in ascending order.
// if data[len(data)-1] < x, return n
func SearchInts(data []int, x int) int {
	lo, hi := 0, len(data)-1
	for lo < hi {
		mid := int(uint(lo+hi) >> 1)
		if data[mid] >= x {
			hi = mid
		} else {
			lo = mid + 1
		}
	}
	return lo
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * sort.go
 *
 *  Created on: Apr 07, 2021
 *      Author: Massimiliano Ghilardi
 */

package sort

type Interface interface {
	Len() int
	Less(i int, j int) bool
	Swap(i int, j int)
}

func Sort(data Interface) {
	n := data.Len()

	for _, gap := range gapsFor(n) {
		for i := gap; i < n; i++ {
			for j := i; j >= gap && data.Less(j, j-gap); j -= gap {
				data.Swap(j, j-gap)
			}
		}
	}
}

func Strings(data []string) {
	n := len(data)
	var i, j int

	for _, gap := range gapsFor(n) {
		for i = gap; i < n; i++ {
			temp := data[i]
			for j = i; j >= gap && temp < data[j-gap]; j -= gap {
				data[j] = data[j-gap]
			}
			data[j] = temp
		}
	}
}

var gaps = makeGaps()

func gapsFor(n int) []int {
	ngap := len(gaps)
	for i := ngap - 1; i > 0; i-- {
		if gaps[i] > n {
			return gaps[i+1 : ngap]
		}
	}
	return gaps
}

func makeGaps() []int {
	list := []int{1, 4, 10, 23, 57, 132, 301, 701}
	n := 1750
	for {
		list = append(list, n)
		if n*2 < 0 {
			break
		}
		n = n*2 + n/4 + 1
	}
	reverse(list)
	return list
}

func reverse(list []int) {
	for i, n := 0, len(list); i < n/2; i++ {
		j := n - i - 1
		list[i], list[j] = list[j], list[i]
	}
}

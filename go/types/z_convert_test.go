/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_convert_test.go
 *
 *  Created on: May 07, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"testing"
)

func testUntypedAssignableTo(k1 Kind, k2 Kind) bool {
	cat1, cat2 := k1.Category(), k2.Category()
	return cat1 == cat2 ||
		(cat1 == Int && cat2 == Float64) ||
		(cat1 == Float64 && cat2 == Int) ||
		(cat1 == UntypedNil && cat2 == UnsafePointer)
}

func testUntypedConvertibleTo(k1 Kind, k2 Kind) bool {
	return testUntypedAssignableTo(k1, k2) ||
		(k1.Category() == Int && k2 == String)
}

func TestAssignableTo(test *testing.T) {
	cs := BasicTypes()
	for _, c1 := range cs {
		if c1 == nil {
			continue
		}
		for _, c2 := range cs {
			if c2 == nil {
				continue
			}
			k1, k2 := c1.Kind(), c2.Kind()
			actual := AssignableTo(c1, c2)
			var ok bool
			if k1.IsUntyped() {
				ok = testUntypedAssignableTo(k1, k2)
			} else {
				ok = k1 == k2
			}
			if actual != ok {
				test.Errorf("AssignableTo(%v, %v) = %v, expecting %v", c1, c2, actual, ok)
			}
		}
	}
}

func TestConvertibleTo(test *testing.T) {
	cs := BasicTypes()
	for _, c1 := range cs {
		if c1 == nil {
			continue
		}
		for _, c2 := range cs {
			if c2 == nil {
				continue
			}
			k1, k2 := c1.Kind(), c2.Kind()
			actual := ConvertibleTo(c1, c2)
			var ok bool
			if k1.IsUntyped() {
				ok = testUntypedConvertibleTo(k1, k2)
			} else if k1 == k2 {
				ok = true
			} else if k2 == String {
				ok = k1.IsInteger()
			} else if k1.IsInteger() || k1.IsFloat() {
				ok = k2.IsInteger() || k2.IsFloat()
			} else if k1.IsComplex() {
				ok = k2.IsComplex()
			} else {
				ok = c1 == c2
			}
			if actual != ok {
				test.Errorf("ConvertibleTo(%v, %v) = %v, expecting %v", c1, c2, actual, ok)
			}
		}
	}
}

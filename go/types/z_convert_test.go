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

func TestAssignableTo(test *testing.T) {
	cs := BasicTypes()
	for _, c1 := range cs {
		for _, c2 := range cs {
			actual := AssignableTo(c1, c2)
			var expected bool
			if c1 == nil || c2 == nil {
				// expected = false
			} else {
				expected = c1.Kind() == c2.Kind()
			}
			if actual != expected {
				test.Errorf("AssignableTo(%v, %v) = %v, expecting %v", c1, c2, actual, expected)
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
			actual := ConvertibleTo(c1, c2)
			var expected bool
			if c1 == nil || c2 == nil {
				// expected = false
			} else if c1.Kind() == c2.Kind() {
				expected = true
			} else if c2.Kind() == String {
				expected = c1.Kind().IsInteger()
			} else if c1.Kind().IsInteger() || c1.Kind().IsFloat() {
				expected = c2.Kind().IsInteger() || c2.Kind().IsFloat()
			} else if c1.Kind().IsComplex() {
				expected = c2.Kind().IsComplex()
			} else {
				expected = c1 == c2
			}
			if actual != expected {
				test.Errorf("ConvertibleTo(%v, %v) = %v, expecting %v", c1, c2, actual, expected)
			}
		}
	}
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * error.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package testutil

import (
	"testing"
)

type ErrorList interface {
	Len() int
	String(int) string
	Error(int) error
}

func CompareErrorsAny(t *testing.T, in string, actual ErrorList, expected_any interface{}) {
	var expected []string
	switch expected_err := expected_any.(type) {
	case string:
		expected = []string{expected_err}
	case []string:
		expected = expected_err
	default:
	}
	CompareErrors(t, in, actual, expected)
}

func CompareErrors(t *testing.T, in string, actual ErrorList, expected []string) {
	actual_n, expected_n := actual.Len(), len(expected)
	if actual_n != expected_n {
		t.Errorf("scan %q returned %d errors, expecting %d", in, actual_n, expected_n)
	}
	n := max2(actual_n, expected_n)
	for i := 0; i < n; i++ {
		var actual_i, expected_i string
		var err_i error
		if i < actual_n {
			actual_i = actual.String(i)
			err_i = actual.Error(i)
		}
		if i < expected_n {
			expected_i = expected[i]
		}
		CompareError(t, in, i, err_i, actual_i, expected_i)
	}
}

func CompareError(t *testing.T, in string, i int, actual_err error, actual_msg string, expected_msg string) {
	if actual_msg != expected_msg {
		t.Errorf("scan %q returned different %d-th error than expected:\n"+
			"\t%v\n"+
			"with error message             : %v\n"+
			"expecting instead error message: %v\n\n",
			in, i, actual_err, actual_msg, expected_msg)
	}
}

func max2(a int, b int) int {
	if a > b {
		return a
	}
	return b
}

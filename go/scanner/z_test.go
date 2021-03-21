/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * z_number_test.go
 *
 *  Created on: Mar 19, 2021
 *      Author: Massimiliano Ghilardi
 */

package scanner

import (
	"strings"
	"testing"
)

type TestCase struct {
	In  string
	Out Item
	Err interface{}
}

type TestCases []TestCase

func (v *TestCases) run(t *testing.T) {
	var reader strings.Reader
	s := NewScanner()
	for _, testcase := range *v {
		testcase.run(t, s, &reader)
	}
}

func (testcase *TestCase) run(t *testing.T, s *Scanner, reader *strings.Reader) {
	s.Reset()
	reader.Reset(testcase.In)
	s.SetReader(reader)

	testcase.expect(t, s)
}

func (testcase *TestCase) expect(t *testing.T, s *Scanner) {
	s.Scan()

	item := testcase.Out
	if s.Tok != item.Tok || s.Lit != item.Lit {
		t.Errorf("scan %q returned {%v %q}, expecting {%v %q}",
			testcase.In, s.Tok, s.Lit, item.Tok, item.Lit)
	}

	actual := s.Errors()
	switch expected := testcase.Err.(type) {
	case error:
		testcase.expectErrors(t, actual, []error{expected})
	case []error:
		testcase.expectErrors(t, actual, expected)
	default:
		testcase.expectErrors(t, actual, nil)
	}

}

func (testcase *TestCase) expectErrors(t *testing.T, actual []error, expected []error) {
	if !sameErrors(actual, expected) {
		t.Errorf("scan %q returned different errors than expected:\n\t%v\nexpecting instead errors:\n\t%v",
			testcase.In, actual, expected)
	}
}

func sameErrors(actual []error, expected []error) bool {
	if len(actual) != len(expected) {
		return false
	}
	for i, act_i := range actual {
		if act_i != expected[i] {
			return false
		}
	}
	return true
}

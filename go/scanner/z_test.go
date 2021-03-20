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
	Out interface{}
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

	switch expected := testcase.Out.(type) {
	case Item:
		testcase.expectItem(t, s, expected)
	default:
		testcase.expectPanic(t, s, expected)
	}
}

func (testcase *TestCase) expectItem(t *testing.T, s *Scanner, expected Item) {
	in := testcase.In
	s.Scan()
	if s.Tok != expected.Tok || s.Lit != expected.Lit {
		t.Errorf("scan %q returned {%v %q}, expecting {%v %q}",
			in, s.Tok, s.Lit, expected.Tok, expected.Lit)
	}
}

func (testcase *TestCase) expectPanic(t *testing.T, s *Scanner, expected interface{}) {
	fail := false
	defer func() {
		if fail {
			return
		}
		actual := recover()
		if actual != expected {
			t.Errorf("scan %q panicked with:\n\t%#v\nexpecting panic with:\n\t%#v",
				testcase.In, actual, expected)
		}
	}()
	s.Scan()
	t.Errorf("scan %q returned {%v %q}, expecting panic with:\n\t%#v",
		testcase.In, s.Tok, s.Lit, expected)
	fail = true
}

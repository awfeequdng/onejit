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
	Err error
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
	in := testcase.In
	item := testcase.Out
	if expected := testcase.Err; expected != nil {
		if err := s.Errors(); len(err) != 1 {
			t.Errorf("scan %q returned different errors than expected:\n\t%v\nexpecting instead error:\n\t%v",
				in, err, expected)
		}
	}
	if s.Tok != item.Tok || s.Lit != item.Lit {
		t.Errorf("scan %q returned {%v %q}, expecting {%v %q}",
			in, s.Tok, s.Lit, item.Tok, item.Lit)
	}
}

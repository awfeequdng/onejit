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
	in string
	Item
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
	reader.Reset(testcase.in)
	s.SetReader(reader)

	tok := s.Scan()
	if tok != testcase.Tok {
		t.Errorf("scan %q returned %v, expecting %v", testcase.in, s.Tok, testcase.Tok)
	}
	if s.Lit != testcase.Lit {
		t.Errorf("scan %q returned %q, expecting %q", testcase.in, s.Lit, testcase.Lit)
	}
}

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

	"github.com/cosmos72/onejit/go/testutil"
	"github.com/cosmos72/onejit/go/token"
)

type (
	SingleTest struct {
		In  string
		Out item
		Err interface{} // one of: nil, error or []error
	}

	MultiTest struct {
		In  string
		Out []item
		Err interface{} // one of: nil, error or []error
	}

	SingleTests []SingleTest
	MultiTests  []MultiTest
)

// --------------------------- SingleTest* -------------------------------------

func (v *SingleTests) run(t *testing.T) {
	var s Scanner
	var reader strings.Reader
	for _, test := range *v {
		test.run(t, &s, &reader)
	}
}

func (test *SingleTest) run(t *testing.T, s *Scanner, reader *strings.Reader) {
	reader.Reset(test.In)
	s.Init(nil, reader)

	test.expect(t, s)
}

func (test *SingleTest) expect(t *testing.T, s *Scanner) {
	actual := makeItem(s.Scan())
	expected := test.Out

	compareItem(t, test.In, 0, actual, expected)
	testutil.CompareErrorsAny(t, test.In, stringList{s.Errors()}, test.Err)
}

// --------------------------- MultiTest* --------------------------------------

func (v *MultiTests) run(t *testing.T) {
	var s Scanner
	var reader strings.Reader
	for _, test := range *v {
		test.run(t, &s, &reader)
	}
}

func (test *MultiTest) run(t *testing.T, s *Scanner, reader *strings.Reader) {
	reader.Reset(test.In)
	s.Init(nil, reader)

	test.expect(t, s)
}

func (test *MultiTest) expect(t *testing.T, s *Scanner) {
	var actual []item
	for {
		tok, lit := s.Scan()
		if tok == token.EOF {
			break
		}
		actual = append(actual, makeItem(tok, lit))
	}
	expected := test.Out

	compareItems(t, test.In, actual, expected)
	testutil.CompareErrorsAny(t, test.In, stringList{s.Errors()}, test.Err)
}

// --------------------------- item --------------------------------------------

func makeItem(tok token.Token, lit string) item {
	return item{tok, lit}
}

func compareItems(t *testing.T, in string, actual []item, expected []item) {
	actual_n, expected_n := len(actual), len(expected)
	if actual_n != expected_n {
		t.Errorf("scan %q returned %d items, expecting %d", in, actual_n, expected_n)
	}
	n := max2(actual_n, expected_n)
	for i := 0; i < n; i++ {
		var actual_i, expected_i item
		if i < actual_n {
			actual_i = actual[i]
		}
		if i < expected_n {
			expected_i = expected[i]
		}
		compareItem(t, in, i, actual_i, expected_i)
	}
}

func compareItem(t *testing.T, in string, i int, actual item, expected item) {
	if actual != expected {
		t.Errorf("scan %q returned {%v %q}, expecting {%v %q}",
			in, actual.tok, actual.lit, expected.tok, expected.lit)
	}
}

// --------------------------- error -------------------------------------------

func max2(a int, b int) int {
	if a > b {
		return a
	}
	return b
}

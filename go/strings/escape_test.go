/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * escape_test.go
 *
 *  Created on: Apr 07, 2021
 *      Author: Massimiliano Ghilardi
 */

package strings

import "testing"

func TestUnescape(t *testing.T) {
	for _, pair := range [...][2]string{
		{``, ""},
		{`foo789`, "foo789"},
		{`\a\b\f\n\r\t\v\\`, "\a\b\f\n\r\t\v\\"},
		{`\000\377`, "\000\377"},
		{`\xab\xFC`, "\xab\xFC"},
		{`\u1234\ufedc`, "\u1234\ufedc"},
		{`\U0010ffff\U000fffff`, "\U0010ffff\U000fffff"},
	} {
		orig := pair[0]
		actual := Unescape(orig)
		expected := pair[1]
		roundtrip := Escape(actual)
		if actual != expected {
			t.Errorf("Unescape returned %q, expecting %q", actual, expected)
		}
		if orig != `\000\377` && ToLower(roundtrip) != ToLower(orig) {
			t.Errorf("Escape returned %q, expecting %q", roundtrip, orig)
		}
	}
}

/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * bad.go
 *
 *  Created on: Mar 23, 2021
 *      Author: Massimiliano Ghilardi
 */

package ast

import (
	"fmt"
)

type Bad struct {
	Atom
}

func (b *Bad) String() string {
	if b == nil {
		return "Bad"
	} else {
		return fmt.Sprint(b)
	}
}

func (b *Bad) Format(out fmt.State, verb rune) {
	if b == nil {
		out.Write([]byte("Bad"))
	} else if len(b.Lit) != 0 {
		fmt.Fprintf(out, "(Bad %v %q)", b.Tok, b.Lit)
	} else {
		fmt.Fprintf(out, "(Bad %v)", b.Tok)
	}
}

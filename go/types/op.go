/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * op.go
 *
 *  Created on: Jun 12, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import "github.com/cosmos72/onejit/go/token"

// return the type of 'e1 op e2' where e1's type = t1 and e2's type = t2
// return nil if t1 and t2 are not compatible with op.
func BinaryOp(t1 *Complete, op token.Token, t2 *Complete) (t *Complete) {
	if t1 == nil || t2 == nil {
		// t = nil
	} else if op == token.SHL || op == token.SHR {
		t = binaryOpShift(t1, t2)
	} else if op >= token.ADD && op <= token.AND_NOT {
		t = binaryOpArith(t1, t2)
	} else if op == token.LAND || op == token.LOR {
		t = binaryOpLandLor(t1, t2)
	} else if op == token.EQL || op == token.LSS || op == token.GTR ||
		op == token.NEQ || op == token.LEQ || op == token.GEQ {
		t = binaryOpComparison(t1, t2)
	}
	return t
}

func binaryOpShift(t1 *Complete, t2 *Complete) (t *Complete) {
	if t1 == nil || t2 == nil {
		return nil
	}
	cat1, cat2 := t1.kind.Category(), t1.kind.Category()
	if cat2 != Int {
		// Go spec: The right operand in a shift expression must have integer type
		// or be an untyped constant representable by a value of type uint
		// t = nil
	} else if cat1 != Int {
		// left operand must have integer type or be an integer untyped constant
	} else if t1.kind.IsUntyped() {
		// Go spec: If the left operand of a non-constant shift expression is an untyped constant,
		// it is first implicitly converted to the type it would assume if the shift expression
		// were replaced by its left operand alone.

		t = t1 // FIXME: currently return the type of left operand (which is untyped)
	} else {
		// TODO
	}
	return t
}

func binaryOpArith(t1 *Complete, t2 *Complete) (t *Complete) {
	// TODO
	return t
}

func binaryOpLandLor(t1 *Complete, t2 *Complete) (t *Complete) {
	// TODO
	return t
}

func binaryOpComparison(t1 *Complete, t2 *Complete) (t *Complete) {
	// TODO
	return t
}

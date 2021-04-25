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
 *  Created on: Apr 25, 2021
 *      Author: Massimiliano Ghilardi
 */

package token

type (
	Error struct {
		Pos Position
		Msg string
	}

	ErrorList []*Error
)

func (e *Error) Error() string {
	if len(e.Pos.Filename) == 0 || !e.Pos.IsValid() {
		return e.Msg
	}
	return e.Pos.String() + ": " + e.Msg
}

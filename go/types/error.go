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
 *  Created on: May 07, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type errorMsg struct {
	msg string
}

func (e errorMsg) Error() string {
	return e.msg
}

var (
	errTypeDifferentKind       = errorMsg{"types have different kind"}
	errTypeIncompatible        = errorMsg{"types are incompatible"} // generic catch-all
	errTypeNamedNotIdentical   = errorMsg{"named types are not identical"}
	errTypeNilUnderlying       = errorMsg{"type has nil underlying type"}
	errUntypedIncompatibleKind = errorMsg{"untyped constant has incompatible kind"}
)

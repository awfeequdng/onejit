/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * interface.go
 *
 *  Created on: Apr 01, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

type Interface struct {
	isInterface struct{} // occupies zero bytes
	rtype       Complete
	extra       extra
}

// *Interface implements Type

func (t *Interface) String() string {
	return t.rtype.str
}

func (t *Interface) Underlying() Type {
	return t
}

func (t *Interface) common() *Complete {
	return &t.rtype
}

// *Interface specific methods

func (t *Interface) NumEmbedded() int {
	return int(t.extra.n1)
}

// return number of declared methods.
// Ignores methods from embedded interfaces
func (t *Interface) NumMethod() int {
	return int(t.extra.n2)
}

// return i-th declared embedded interface.
func (t *Interface) Embedded(i int) Type {
	return t.extra.fields[0:t.extra.n1][i].Type
}

// return i-th declared method.
// Ignores methods from embedded interfaces
func (t *Interface) Method(i int) Method {
	field := &t.extra.fields[t.extra.n1:][i]
	return Method{
		Type:          field.Type,
		QualifiedName: field.QualifiedName,
		Index:         i,
	}
}

/*
 * gomacrojit - JIT compiler in Go
 *
 * Copyright (C) 2018-2020-2020 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * constraint.go
 *
 *  Created on Dec 26, 2020
 *      Author Massimiliano Ghilardi
 */

package jit

type Constraint interface {
	Class() Class
	Children() int
	Child(i int) Constraint
}

// ============================ ClassConstraint ================================

type ClassConstraint struct {
	FClass    Class
	FChildren []Constraint
}

func (cc *ClassConstraint) Class() Class {
	return cc.FClass
}

func (cc *ClassConstraint) Children() int {
	return len(cc.FChildren)
}

func (cc *ClassConstraint) Child(i int) Constraint {
	return cc.FChildren[i]
}

// ============================ ConstConstraint ================================

type ConstConstraint struct {
	FPred func(c Const) bool
}

func (cc ConstConstraint) Class() Class {
	return CONST
}

func (cc ConstConstraint) Children() int {
	return 0
}

func (cc ConstConstraint) Child(i int) Constraint {
	badIndex(i, 0)
	return nil
}

// ============================ RegConstraint ==================================

type RegConstraint struct {
	FLo RegId
	FHi RegId
}

func (rc RegConstraint) Class() Class {
	return REG
}

func (rc RegConstraint) Children() int {
	return 0
}

func (rc RegConstraint) Child(i int) Constraint {
	badIndex(i, 0)
	return nil
}

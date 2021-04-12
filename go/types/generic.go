/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * generic.go
 *
 *  Created on: Apr 12, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"github.com/cosmos72/onejit/go/io"
	"github.com/cosmos72/onejit/go/strings"
)

type (
	// Constraint represents a constraint on a generic type.
	// it is either a named or unnamed interface type (i.e. .Kind() = InterfaceKind)
	// or a generic type that, when instantiated, declares a named or unnamed interface type
	Constraint struct {
		typ     Type
		generic *Generic
	}

	// Generic represents a generic type
	Generic struct {
		name        string
		pkgPath     string
		types       []string
		constraints []Constraint
		instantiate Instantiate
		fullStr     string
		shortStr    string
	}

	// user-provided function to instantiate a generic type
	Instantiate = func(*Generic, []Type) Type
)

func NewConstraintType(typ Type) *Constraint {
	return &Constraint{typ: typ}
}

func NewConstraintGeneric(generic *Generic) *Constraint {
	return &Constraint{generic: generic}
}

// *Generic does NOT implement Type

func (g *Generic) String() string {
	var b builder
	g.WriteTo(&b, fullPkgPath)
	return b.String()
}

func (g *Generic) WriteTo(dst io.StringWriter, flag verbose) {
	var str string
	if flag == shortPkgName {
		str = g.shortStr
	} else {
		str = g.fullStr
	}
	dst.WriteString(str)
}

// *Generic specific methods

func (g *Generic) Name() string {
	return g.name
}

func (g *Generic) PkgPath() string {
	return g.pkgPath
}

func (g *Generic) NumTypes() int {
	return len(g.types)
}

func (g *Generic) TypeParam(i int) string {
	return g.types[i]
}

func (g *Generic) TypeConstraint(i int) Constraint {
	return g.constraints[i]
}

func (g *Generic) Instantiate(concreteTypes []Type) Type {
	return g.instantiate(g, concreteTypes)
}

// Declare a new Generic type. A generic type *cannot* be an alias
func NewGeneric(
	name string, pkgPath string, paramNames []string, constraints []Constraint, instantiate Instantiate,
) *Generic {

	if len(paramNames) != len(constraints) {
		panic("NewGeneric: paramNames and constraints have different lengths")
	} else if instantiate == nil {
		panic("NewGeneric: nil instantiate")
	}
	fullStr := makeGenericFullString(name, pkgPath, paramNames)
	shortStr := makeGenericShortString(pkgPath, fullStr)
	return &Generic{
		name:        name,
		pkgPath:     fullStr[0:len(pkgPath)],
		types:       paramNames,
		constraints: constraints,
		instantiate: instantiate,
		fullStr:     fullStr,
		shortStr:    shortStr,
	}
}

func makeGenericFullString(name string, pkgPath string, paramNames []string) string {
	var b builder
	if len(pkgPath) != 0 {
		b.WriteString(pkgPath)
		b.WriteByte('.')
	}
	b.WriteString(name)
	b.WriteByte('[')
	for i, paramName := range paramNames {
		if i != 0 {
			b.WriteString(", ")
		}
		b.WriteString(paramName)
	}
	b.WriteByte(']')
	return b.String()
}

func makeGenericShortString(pkgPath string, fullStr string) string {
	offset := strings.LastIndexByte(pkgPath, '/') + 1
	return fullStr[offset:]
}

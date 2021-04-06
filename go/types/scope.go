/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * scope.go
 *
 *  Created on: Apr 05, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import (
	"sort"

	"github.com/cosmos72/onejit/go/arch"
)

type Class uint8

const (
	InvalidObj Class = iota
	BuiltinObj
	ConstObj
	FuncObj
	TypeObj
	VarObj
)

func (cl Class) String() string {
	var str string
	switch cl {
	case BuiltinObj:
		str = "builtin"
	case ConstObj:
		str = "const"
	case FuncObj:
		str = "func"
	case TypeObj:
		str = "type"
	case VarObj:
		str = "var"
	default:
		str = "invalid"
	}
	return str
}

// --------------------------- Object ------------------------------------------

type Object struct {
	cls  Class
	name string
	typ  *Complete
}

func (obj *Object) Class() Class {
	return obj.cls
}

func (obj *Object) Name() string {
	return obj.name
}

func (obj *Object) Type() *Complete {
	return obj.typ
}

func (obj *Object) checkValid() {
	if obj == nil {
		panic("Object is nil")
	} else if len(obj.Name()) == 0 {
		panic("Object has empty name")
	}
	t := obj.Type()
	if t == nil {
		panic("Object has nil type")
	}
	switch obj.Class() {
	case BuiltinObj:
		if !isBuiltinType(t) {
			panic("type of BuiltinObj must be created with NewBuiltin")
		}
		return
	case ConstObj:
		if !t.kind.IsBasic() {
			panic("type of ConstObj must be one of the basic types")
		}
	case FuncObj:
		if t.kind != FuncKind {
			panic("type of FuncObj must be a function")
		} else if len(t.Name()) != 0 {
			panic("type of FuncObj must be unnamed")
		}
	case TypeObj:
		// nothing to do
	case VarObj:
		if t.kind.IsUntyped() {
			panic("type of VarObj cannot be untyped.*")
		}
	default:
		panic("Object has invalid class")
	}
	if isBuiltinType(t) {
		panic("type of " + obj.cls.String() + " cannot be builtin")
	}
}

func isBuiltinType(t *Complete) bool {
	return t.typ == nil && t.extra != nil
}

// --------------------------- Scope -------------------------------------------

type Scope struct {
	parent *Scope
	m      map[string]*Object
}

func NewScope(parent *Scope) *Scope {
	return &Scope{parent: parent}
}

// insert obj into Scope s. overwrites any existing object with the same name
func (s *Scope) Insert(obj *Object) {
	obj.checkValid()
	if s.m == nil {
		s.m = make(map[string]*Object)
	}
	s.m[obj.name] = obj
}

// search object by name in specified Scope.
func (s *Scope) Lookup(name string) *Object {
	return s.m[name]
}

// search object by name in specified Scope and all its parent scopes.
func (s *Scope) LookupParent(name string) (*Scope, *Object) {
	for ; s != nil; s = s.parent {
		if obj := s.m[name]; obj != nil {
			return s, obj
		}
	}
	return nil, nil
}

// return the sorted names of all objects in Scope
func (s *Scope) Names() []string {
	names := make([]string, len(s.m))
	i := 0
	for name := range s.m {
		names[i] = name
		i++
	}
	if len(names) > 1 {
		sort.Strings(names)
	}
	return names
}

func (s *Scope) Parent() *Scope {
	return s.parent
}

// --------------------------- Universe -------------------------------------------

var (
	universeLinux386 = makeUniverse(basicTypesLinux386)
	universeOther32  = makeUniverse(basicTypesOther32)
	universe64       = makeUniverse(basicTypes64)
)

// return the top-level scope, containing all predeclared objects of Go.
// result depends on arch.Target()
func Universe() *Scope {
	var universe *Scope
	os, arc := arch.Target()
	if arc.Bits() > 32 {
		universe = universe64
	} else if os == arch.Linux && arc == arch.I386 {
		universe = universeLinux386
	} else {
		universe = universeOther32
	}
	return universe
}

func makeUniverse(basic []*Complete) *Scope {
	s := Scope{}
	for _, b := range basic[:Complex128+1] {
		if b != nil {
			s.Insert(&Object{TypeObj, b.Name(), b})
		}
	}
	errorType := NewNamed("error", "")
	errorType.SetUnderlying(NewInterface(nil, Method{
		Name: "Error",
		Type: NewFunc(nil, []Type{basic[String].Type()}, false),
	}))

	objs := []Object{
		// each BuiltinObj has a unique type
		{BuiltinObj, "append", NewBuiltin(2, 1, true)},
		{BuiltinObj, "cap", NewBuiltin(1, 0, false)},
		{BuiltinObj, "close", NewBuiltin(1, 0, false)},
		{BuiltinObj, "complex", NewBuiltin(2, 1, false)},
		{BuiltinObj, "copy", NewBuiltin(2, 1, false)},
		{BuiltinObj, "delete", NewBuiltin(2, 0, false)},
		{BuiltinObj, "imag", NewBuiltin(1, 1, false)},
		{BuiltinObj, "len", NewBuiltin(1, 1, false)},
		{BuiltinObj, "make", NewBuiltin(2, 1, true)},
		{BuiltinObj, "new", NewBuiltin(1, 1, true)},
		{BuiltinObj, "panic", NewBuiltin(1, 0, false)},
		{BuiltinObj, "print", NewBuiltin(1, 0, true)},
		{BuiltinObj, "println", NewBuiltin(1, 0, true)},
		{BuiltinObj, "real", NewBuiltin(1, 1, false)},
		{BuiltinObj, "recover", NewBuiltin(0, 1, false)},

		{ConstObj, "false", basic[UntypedBool]},
		{ConstObj, "iota", basic[UntypedInt]},
		{ConstObj, "nil", basic[UntypedNil]},
		{ConstObj, "true", basic[UntypedBool]},

		{TypeObj, "byte", basic[Uint8]},
		{TypeObj, "rune", basic[Int32]},
		{TypeObj, "error", errorType.common()},
	}
	for i := range objs {
		s.Insert(&objs[i])
	}
	return &s
}
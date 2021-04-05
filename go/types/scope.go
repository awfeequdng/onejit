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

import "sort"

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
	Class Class
	Name  string
	Type  *Complete
}

func (obj *Object) Valid() bool {
	return len(obj.Name) != 0
}

func (obj *Object) checkValid() {
	if len(obj.Name) == 0 {
		panic("Object has empty name")
	} else if obj.Type == nil {
		panic("Object has nil type")
	}
	switch obj.Class {
	case BuiltinObj:
		if !isBuiltinType(obj.Type) {
			panic("type of BuiltinObj must be created with NewBuiltin")
		}
		return
	case ConstObj:
		if !obj.Type.kind.IsBasic() {
			panic("type of ConstObj must be one of the basic types")
		}
	case FuncObj:
		if obj.Type.kind != FuncKind {
			panic("type of FuncObj must be a function")
		} else if len(obj.Type.Name()) != 0 {
			panic("type of FuncObj must be unnamed")
		}
	case TypeObj, VarObj:
		// nothing to do
	default:
		panic("Object has invalid class")
	}
	if isBuiltinType(obj.Type) {
		panic("type of " + obj.Class.String() + " cannot be builtin")
	}
}

func isBuiltinType(t *Complete) bool {
	b, _ := t.typ.(*Basic)
	return b != nil && t.extra != nil
}

// --------------------------- Scope -------------------------------------------

type Scope struct {
	parent *Scope
	m      map[string]Object
}

func NewScope(parent *Scope) *Scope {
	return &Scope{parent: parent}
}

// insert obj into Scope s. overwrites any existing object with the same name
func (s *Scope) Insert(obj Object) {
	obj.checkValid()
	if s.m == nil {
		s.m = make(map[string]Object)
	}
	s.m[obj.Name] = obj
}

// search object by name in specified Scope.
// if no object is found, return Object{}
func (s *Scope) Lookup(name string) Object {
	return s.m[name]
}

// search object by name in specified Scope and all its parent scopes.
// if no object is found, return Object{}
func (s *Scope) LookupParent(name string) (*Scope, Object) {
	for ; s != nil; s = s.parent {
		if obj := s.m[name]; obj.Valid() {
			return s, obj
		}
	}
	return nil, Object{}
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
	universe32 = makeUniverse(basicTypes32)
	universe64 = makeUniverse(basicTypes64)
)

// contains all predeclared objects of Go
func Universe() *Scope {
	universe := universe32
	if archSizeBits > ArchSize32 {
		universe = universe64
	}
	return universe
}

func makeUniverse(basic []*Basic) *Scope {
	s := Scope{}
	for _, b := range basic {
		if b != nil {
			s.Insert(Object{TypeObj, b.Name(), b.Complete()})
		}
	}
	s.Insert(Object{TypeObj, "byte", basic[Uint8].Complete()})
	s.Insert(Object{TypeObj, "rune", basic[Int32].Complete()})
	errorType := NewNamed("error", "")
	errorType.SetUnderlying(NewInterface(nil, Method{
		Name: "Error",
		Type: NewFunc(nil, []Type{basic[String]}, false),
	}))
	s.Insert(Object{TypeObj, "error", errorType.common()})
	s.Insert(Object{BuiltinObj, "append", NewBuiltin(2, 1, true).Complete()})
	s.Insert(Object{BuiltinObj, "cap", NewBuiltin(1, 0, false).Complete()})
	s.Insert(Object{BuiltinObj, "close", NewBuiltin(1, 0, false).Complete()})
	s.Insert(Object{BuiltinObj, "complex", NewBuiltin(2, 1, false).Complete()})
	s.Insert(Object{BuiltinObj, "copy", NewBuiltin(2, 1, false).Complete()})
	s.Insert(Object{BuiltinObj, "delete", NewBuiltin(2, 0, false).Complete()})
	s.Insert(Object{BuiltinObj, "imag", NewBuiltin(1, 1, false).Complete()})
	s.Insert(Object{BuiltinObj, "len", NewBuiltin(1, 1, false).Complete()})
	s.Insert(Object{BuiltinObj, "make", NewBuiltin(2, 1, true).Complete()})
	s.Insert(Object{BuiltinObj, "new", NewBuiltin(1, 1, true).Complete()})
	s.Insert(Object{BuiltinObj, "panic", NewBuiltin(1, 0, false).Complete()})
	s.Insert(Object{BuiltinObj, "print", NewBuiltin(1, 0, true).Complete()})
	s.Insert(Object{BuiltinObj, "println", NewBuiltin(1, 0, true).Complete()})
	s.Insert(Object{BuiltinObj, "real", NewBuiltin(1, 1, false).Complete()})
	s.Insert(Object{BuiltinObj, "recover", NewBuiltin(0, 1, false).Complete()})

	// TODO: untyped constants false, iota, nil, true
	return &s
}

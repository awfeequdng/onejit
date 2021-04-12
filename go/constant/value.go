/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * value.go
 *
 *  Created on: Apr 12, 2021
 *      Author: Massimiliano Ghilardi
 */

package constant

import (
	"go/constant"
	"math/big"
)

// wraps go/constant.Value
type Value struct {
	val  constant.Value
	kind Kind
}

func (c *Value) Kind() Kind {
	return c.kind
}

/**
 * get the value of a constant. returned type is:
 * bool    if Kind is Bool or UntypedBool
 * string  if Kind is String or UntypedString
 * int64   if kind is Int*
 * uint64  if kind is Uint*
 * int64 or *math/big.Int if kind is UntypedInt or UntypedRune
 * *big.Float or *big.Rat if kind is Float* or UntypedFloat
 * nil     if Kind is Invalid, Complex*, UntypedComplex or UntypedNil
 */
func (c *Value) Value() (val interface{}) {
	switch c.kind {
	case Bool, UntypedBool:
		val = constant.BoolVal(c.val)
	case Int, Int8, Int16, Int32, Int64:
		val, _ = constant.Int64Val(c.val)
	case Uint, Uint8, Uint16, Uint32, Uint64, Uintptr:
		i, _ := constant.Int64Val(c.val)
		val = uint64(i)
	case String, UntypedString:
		val = constant.StringVal(c.val)
	case Invalid, Complex64, Complex128, UntypedComplex, UntypedNil:
		val = nil
	default:
		val = constant.Val(c.val)
	}
	return val
}

/**
 * Create a constant. Allowed val types are:
 * bool    if Kind is Bool or UntypedBool
 * string  if Kind is String or UntypedString
 * int64   if kind is Int*
 * uint64  if kind is Uint*
 * int64 or *math/big.Int if kind is UntypedInt or UntypedRune
 * float64, *big.Float or *big.Rat if kind is Float* or UntypedFloat
 * nil     if Kind is Invalid or UntypedNil
 *
 * panics if Kind is Complex*, UntypedComplex or something else.
 */
func Make(kind Kind, val interface{}) *Value {
	var cval constant.Value
	switch kind {
	case Bool, UntypedBool:
		cval = constant.MakeBool(val.(bool))
	case Int, Int8, Int16, Int32, Int64:
		cval = constant.MakeInt64(val.(int64))
	case Uint, Uint8, Uint16, Uint32, Uint64, Uintptr:
		cval = constant.MakeInt64(int64(val.(uint64)))
	case String, UntypedString:
		val = constant.MakeString(val.(string))
	case UntypedInt, UntypedRune:
		switch val := val.(type) {
		case int64:
			cval = constant.MakeInt64(val)
		default:
			cval = constant.Make(val.(*big.Int))
		}
	case Float32, Float64, UntypedFloat:
		switch val := val.(type) {
		case float64:
			cval = constant.MakeFloat64(val)
		case *big.Rat:
			cval = constant.Make(val)
		default:
			cval = constant.Make(val.(*big.Float))
		}
	case Invalid, UntypedNil:
		if val != nil {
			panic("constant.Make: non-nil value with kind = " + kind.String())
		}
		val = constant.MakeUnknown()
	}
	return &Value{cval, kind}
}

/**
 * Create the complex constant i * val
 * If c.Kind() is Int*, Uint* or Float*, complexKind must be Complex64 or Complex128
 * if c.Kind() is UntypedInt, UntypedRune or UntypedFloat, complexKind must be UntypedComplex
 * otherwise complexKind must be Invalid
 */
func MakeImag(complexKind Kind, c *Value) *Value {
	switch complexKind {
	case Complex64, Complex128:
		switch c.Kind() {
		case Int, Int8, Int16, Int32, Int64,
			Uint, Uint8, Uint16, Uint32, Uint64, Uintptr,
			Float32, Float64:
		default:
			panic("constant.MakeImag: complexKind = " + complexKind.String() +
				" requires an existing constant with Kind = Int, Int8, Int16, Int32, Int64, " +
				"Uint, Uint8, Uint16, Uint32, Uint64, Uintptr, Float32 or Float64")
		}
	case UntypedComplex:
		switch c.Kind() {
		case UntypedInt, UntypedRune, UntypedFloat:
		default:
			panic("constant.MakeImag: complexKind = " + complexKind.String() +
				" requires an existing constant with Kind = UntypedInt, UntypedRune or UntypedFloat")
		}
	case Invalid:
		switch c.Kind() {
		case Invalid:
		default:
			panic("constant.MakeImag: complexKind = " + complexKind.String() +
				" requires an existing constant with Kind = Invalid")
		}
	}
	return &Value{constant.MakeImag(c.val), complexKind}
}

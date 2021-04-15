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

	"github.com/cosmos72/onejit/go/token"
)

// wraps go/constant.Value
type Value struct {
	cval constant.Value
	kind Kind
}

/**
 * return a short, quoted, user-readable string representation of v;
 * v.Kind() must be String or Invalid
 */
func (v *Value) String() string {
	return "{" + v.kind.String() + " " + v.cval.String() + "}"
}

func (v *Value) Kind() Kind {
	if v == nil {
		return Invalid
	}
	return v.kind
}

/**
 * get the value of a constant. returned type is:
 * bool    if Kind is Bool
 * string  if Kind is String
 * int64 or *math/big.Int if kind is Int or Rune
 * *big.Float or *big.Rat if kind is Float
 * nil     if Kind is Invalid, Complex or Nil
 */
func (v *Value) Value() interface{} {
	return constant.Val(v.cval)
}

/**
 * return the bool value of v.
 * v.Kind() must be Bool or Invalid
 */
func (v *Value) Bool() bool {
	return constant.BoolVal(v.cval)
}

/**
 * return the int64 value of v and whether the result is exact;
 * v.Kind() must be Int, Rune or Invalid
 */
func (v *Value) Int64() (int64, bool) {
	return constant.Int64Val(v.cval)
}

/**
 * return the int64 value of v and whether the result is exact;
 * v.Kind() must be Int, Rune or Invalid
 */
func (v *Value) Uint64() (uint64, bool) {
	return constant.Uint64Val(v.cval)
}

/**
 * return the float32 value of v and whether the result is exact;
 * v.Kind() must be Int, Rune, Float or Invalid
 */
func (v *Value) Float32() (float32, bool) {
	return constant.Float32Val(v.cval)
}

/**
 * return the float32 value of v and whether the result is exact;
 * v.Kind() must be Int, Rune, Float or Invalid
 */
func (v *Value) Float64() (float64, bool) {
	return constant.Float64Val(v.cval)
}

/**
 * return the complex64 value of v and whether the result is exact;
 * v.Kind() must be Int, Rune, Float, Complex or Invalid
 */
func (v *Value) Complex64() (complex64, bool) {
	re, rexact := v.Real().Float32()
	im, iexact := v.Imag().Float32()
	return complex(re, im), rexact && iexact
}

/**
 * return the complex128 value of v and whether the result is exact;
 * v.Kind() must be Int, Rune, Float, Complex or Invalid
 */
func (v *Value) Complex128() (complex128, bool) {
	re, rexact := v.Real().Float64()
	im, iexact := v.Imag().Float64()
	return complex(re, im), rexact && iexact
}

/**
 * return the string value of v;
 * v.Kind() must be String or Invalid
 */
func (v *Value) StringVal() string {
	return constant.StringVal(v.cval)
}

/**
 * Create a constant. Allowed val types are:
 * bool    if Kind is Bool
 * string  if Kind is String
 * int64 or *math/big.Int if kind is Int or Rune
 * float64, *big.Float or *big.Rat if kind is Float
 * nil     if Kind is Invalid or Nil
 *
 * panics if Kind is Complex or something else.
 */
func Make(kind Kind, x interface{}) *Value {
	var c constant.Value
	switch kind {
	case Bool:
		c = constant.MakeBool(x.(bool))
	case String:
		c = constant.MakeString(x.(string))
	case Int, Rune:
		switch x := x.(type) {
		case int64:
			c = constant.MakeInt64(x)
		default:
			c = constant.Make(x.(*big.Int))
		}
	case Float:
		switch x := x.(type) {
		case float64:
			c = constant.MakeFloat64(x)
		case *big.Rat:
			c = constant.Make(x)
		default:
			c = constant.Make(x.(*big.Float))
		}
	case Complex:
		panic("constant.Make: kind = " + kind.String() + " is not supported, use constant.MakeImag() instead")
	case Invalid, Nil:
		if x != nil {
			panic("constant.Make: non-nil value with kind = " + kind.String())
		}
		c = constant.MakeUnknown()
	default:
		panic("constant.Make: unsupported kind = " + kind.String())
	}
	return &Value{c, kind}
}

/**
 * Create the complex constant i * c
 */
func MakeImag(v *Value) *Value {
	ret := Value{nil, Complex}
	switch v.kind {
	case Int, Rune, Float:
		ret.cval = constant.MakeImag(v.cval)
	case Invalid:
		ret.kind = Invalid
	default:
		panic("constant.MakeImag: unsupported kind = " + v.kind.String())
	}
	return &ret
}

// return numerator of Value v.
// v.Kind() must be Int, Rune, Float or Invalid
func (v *Value) Num() *Value {
	c := constant.Num(v.cval)
	kind := v.kind
	if kind != Rune {
		kind = approxKind(c)
	}
	return &Value{c, kind}
}

// return denominator of Value.
// v.Kind() must be Int, Rune, Float or Invalid
func (v *Value) Den() *Value {
	c := constant.Denom(v.cval)
	kind := v.kind
	if kind != Rune {
		kind = approxKind(c)
	}
	return &Value{c, kind}
}

// return real part of Value v.
// v.Kind() must be Int, Rune, Float, Complex or Invalid
func (v *Value) Real() *Value {
	c := constant.Real(v.cval)
	kind := v.kind
	if kind != Rune {
		kind = approxKind(c)
	}
	return &Value{c, kind}
}

// return imaginary part of Value v.
// v.Kind() must be Int, Rune, Float, Complex or Invalid
func (v *Value) Imag() *Value {
	val := constant.Imag(v.cval)
	return &Value{val, approxKind(val)}
}

// return approximate kind of go/constant.Value.
// approximate because it cannot deduce Rune
func approxKind(c constant.Value) Kind {
	kind := Invalid
	switch c.Kind() {
	case constant.Bool:
		kind = Bool
	case constant.String:
		kind = String
	case constant.Int:
		kind = Int
	case constant.Float:
		kind = Float
	case constant.Complex:
		kind = Complex
	}
	return kind
}

func ToComplex(v *Value) *Value {
	return &Value{constant.ToComplex(v.cval), Complex}
}

func ToFloat(v *Value) *Value {
	return &Value{constant.ToFloat(v.cval), Float}
}

func ToInt(v *Value) *Value {
	return &Value{constant.ToInt(v.cval), Int}
}

func ToRune(v *Value) *Value {
	return &Value{constant.ToInt(v.cval), Rune}
}

/**
 * try to convert Value v to to specified kind.
 * if conversion is not allowed returns nil, ErrorBadKind.
 * if conversion is not exact returns nil, ErrorOverflow.
 * if conversion is allowed and exact, returned error is nil, and returned interface{} is:
 * bool       if kind is token.Bool
 * int64      if kind is token.Int*
 * uint64     if kind is token.Uint*
 * float32    if kind is token.Float32
 * float64    if kind is token.Float64
 * complex64  if kind is token.Complex64
 * complex128 if kind is token.Complex128
 * string     if kind is token.String
 */
func (v *Value) ToTyped(kind Kind) (interface{}, error) {
	var ret interface{}
	var err error
	badkind, exact := false, false

	ckind := v.cval.Kind()
	if ckind == constant.Unknown {
		return nil, ErrInvalid
	}
	switch kind {
	case token.Bool:
		if ckind == constant.Bool {
			ret = v.Bool()
			exact = true
		} else {
			badkind = true
		}
	case token.Int, token.Int8, token.Int16, token.Int32, token.Int64:
		if ckind == constant.Int {
			i, iexact := v.Int64()
			if iexact && int64fits(i, kind) {
				ret = i
				exact = true
			}
		} else {
			badkind = true
		}
	case token.Uint, token.Uint8, token.Uint16, token.Uint32, token.Uint64, token.Uintptr:
		if ckind == constant.Int {
			n, nexact := v.Uint64()
			if nexact && uint64fits(n, kind) {
				ret = n
				exact = true
			} else {
				exact = false
			}
		} else {
			badkind = true
		}
	case token.Float32:
		if ckind == constant.Int || ckind == constant.Float {
			ret, exact = v.Float32()
		} else {
			badkind = true
		}
	case token.Float64:
		if ckind == constant.Int || ckind == constant.Float {
			ret, exact = v.Float64()
		} else {
			badkind = true
		}
	case token.Complex64:
		if ckind == constant.Int || ckind == constant.Float || ckind == constant.Complex {
			ret, exact = v.Complex64()
		} else {
			badkind = true
		}
	case token.Complex128:
		if ckind == constant.Int || ckind == constant.Float || ckind == constant.Complex {
			ret, exact = v.Complex128()
		} else {
			badkind = true
		}
	case token.String:
		if ckind == constant.String {
			ret = v.StringVal()
			exact = true
		} else {
			badkind = true
		}
	}
	if badkind {
		err = ErrorKind{v.cval, kind}
	} else if !exact {
		err = ErrorOverflow{v.cval, kind}
		ret = nil
	}
	return ret, err
}

func int64fits(i int64, kind Kind) bool {
	bits := kind.Size() * 8
	mask := ^uint64(0) >> (65 - bits)
	if i < 0 {
		i = ^i
	}
	return uint64(i) <= mask
}

func uint64fits(n uint64, kind Kind) bool {
	bits := kind.Size() * 8
	mask := ^uint64(0) >> (64 - bits)
	return n <= mask
}

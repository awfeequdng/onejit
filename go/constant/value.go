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
	"fmt"
	"go/constant"
	"go/token"
	"math/big"
)

type value struct {
	cval constant.Value
	kind Kind
	err  error
}

// equivalent to go/constant.Value
// the zero value i.e. Value{} has .Kind() = Invalid and .Err() = ErrInvalid
type Value struct {
	*value
}

var (
	cunknown = constant.MakeUnknown()

	invalid    = Value{&value{cunknown, Invalid, ErrInvalid}}
	untypedNil = Value{&value{cunknown, UntypedNil, nil}}
)

func (v Value) Kind() Kind {
	if v.value == nil {
		return Invalid
	}
	return v.kind
}

/** return a short, quoted, user-readable string representation of v */
func (v Value) String() string {
	if v.value == nil {
		return invalid.String()
	} else if v.kind == Invalid {
		if v.err != nil {
			return "{error: " + v.err.Error() + "}"
		} else {
			return v.kind.String()
		}
	} else if v.kind == UntypedNil {
		return v.kind.String()
	} else if v.kind == UntypedRune {
		return "{" + v.kind.String() + " " + untypedRune2string(v.cval) + "}"
	} else if v.kind.IsUntyped() {
		return "{" + v.kind.String() + " " + v.cval.String() + "}"
	} else {
		return v.cval.String()
	}
}

/**
 * get the value of a constant. returned type is:
 * bool    if Kind is Bool
 * string  if Kind is String
 * int64 or *math/big.Int if kind is Int*, Uint*, UntypedInt or UntypedRune
 * *big.Float or *big.Rat if kind is Float* or UntypedFloat
 * nil     if Kind is Invalid, Complex*, UntypedComplex or UntypedNil
 */
func (v Value) Value() interface{} {
	if v.value == nil {
		return nil
	}
	return constant.Val(v.cval)
}

/**
 * IsValid returns true if a Value is valid i.e. if its kind is != Invalid.
 */
func (v Value) IsValid() bool {
	return v.value != nil && v.kind != Invalid
}

/**
 * Err returns the error of an Invalid constant.
 * returns nil if the constant is valid.
 */
func (v Value) Err() error {
	if v.value == nil {
		return ErrInvalid
	}
	return v.err
}

/**
 * Bool returns the bool value of v.
 * v.Kind() must be Bool or Invalid
 */
func (v Value) Bool() bool {
	if v.value == nil {
		return false
	}
	return constant.BoolVal(v.cval)
}

/**
 * Int64 returns the int64 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, UntypedInt, UntypedRune or Invalid
 */
func (v Value) Int64() (int64, bool) {
	if v.value == nil {
		return 0, false
	}
	return constant.Int64Val(v.cval)
}

/**
 * Uint64 returns the int64 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, UntypedInt, UntypedRune or Invalid
 */
func (v Value) Uint64() (uint64, bool) {
	if v.value == nil {
		return 0, false
	}
	return constant.Uint64Val(v.cval)
}

/**
 * Float32 returns the float32 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, Float*, UntypedInt, UntypedRune, UntypedFloat or Invalid
 */
func (v Value) Float32() (float32, bool) {
	if v.value == nil {
		return 0, false
	}
	return constant.Float32Val(v.cval)
}

/**
 * Float64 returns the float64 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, Float*, UntypedInt, UntypedRune, UntypedFloat or Invalid
 */
func (v Value) Float64() (float64, bool) {
	if v.value == nil {
		return 0, false
	}
	return constant.Float64Val(v.cval)
}

/**
 * Complex64 returns the complex64 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, Float*, Complex*,
 * UntypedInt, UntypedRune, UntypedFloat, UntypedComplex or Invalid
 */
func (v Value) Complex64() (complex64, bool) {
	if v.value == nil {
		return 0, false
	}
	re, rexact := v.Real().Float32()
	im, iexact := v.Imag().Float32()
	return complex(re, im), rexact && iexact
}

/**
 * Complex128 returns the complex128 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, Float*, Complex*,
 * UntypedInt, UntypedRune, UntypedFloat, UntypedComplex or Invalid
 */
func (v Value) Complex128() (complex128, bool) {
	if v.value == nil {
		return 0, false
	}
	re, rexact := v.Real().Float64()
	im, iexact := v.Imag().Float64()
	return complex(re, im), rexact && iexact
}

/**
 * StringVal returns the string value of v;
 * v.Kind() must be String, UntypedString or Invalid
 */
func (v Value) StringVal() (string, bool) {
	if v.value == nil || v.cval.Kind() != constant.String {
		return "", false
	}
	return constant.StringVal(v.cval), true
}

/**
 * Make creates a constant. Allowed x types depend on kind:
 * bool    if Kind is Bool or UntypedBool
 * string  if Kind is String or UntypedString
 * int*, uint* or *math/big.Int if kind is Int*, Uint*, UntypedInt or UntypedRune
 * float32, float64, *big.Float or *big.Rat if kind is Float* or UntypedFloat
 * complex64 or complex128 if kind is Complex* or UntypedComplex
 * nil     if Kind is Invalid or UntypedNil
 *
 * return Value{Invalid, ErrKind} if conversion of val to kind is not allowed.
 * return Value{Invalid, ErrOverflow} if conversion of val to kind is not exact.
 */
func Make(kind Kind, x interface{}) Value {
	var c constant.Value
	needvalidate := !kind.IsUntyped()
	badkind := false

	switch kind {
	case Invalid:
		if x == nil {
			return invalid
		}
		badkind = true
	case Bool, UntypedBool:
		switch x := x.(type) {
		case bool:
			c = constant.MakeBool(x)
			needvalidate = false
		default:
			badkind = true
		}
	case Int, Int8, Int16, Int32, Int64,
		Uint, Uint8, Uint16, Uint32, Uint64, Uintptr,
		UntypedInt, UntypedRune:

		if bx, ok := x.(*big.Int); ok {
			if bx.IsInt64() {
				// Go < 1.16 does not like *big.Int containing small numbers
				c = constant.MakeInt64(bx.Int64())
			} else {
				c = constant.Make(bx)
			}
		} else {
			c, badkind = makeInt(x)
		}
	case Float32, Float64, UntypedFloat:
		switch x := x.(type) {
		case float32:
			c = constant.MakeFloat64(float64(x))
		case float64:
			c = constant.MakeFloat64(x)
		case *big.Rat, *big.Float:
			c = constant.Make(x)
		default:
			badkind = true
		}
	case Complex64, Complex128, UntypedComplex:
		var cplx complex128
		switch x := x.(type) {
		case complex64:
			cplx = complex128(x)
		case complex128:
			cplx = x
		default:
			badkind = true
		}
		if badkind {
			break
		}
		cre := constant.MakeFloat64(real(cplx))
		cim := constant.MakeFloat64(imag(cplx))
		cre = constant.ToComplex(cre)
		cim = constant.MakeImag(cim)
		c = constant.BinaryOp(cre, token.ADD, cim)

	case String, UntypedString:
		switch x := x.(type) {
		case string:
			c = constant.MakeString(x)
			needvalidate = false
		default:
			badkind = true
		}
	case UntypedNil:
		if x == nil {
			return untypedNil
		}
		badkind = true
	default:
		badkind = true
	}
	var v Value
	if badkind {
		v = Value{&value{cunknown, Invalid, ErrorKind{fmt.Sprint(x), kind}}}
	} else if needvalidate {
		v = validate(c, kind)
	} else {
		v = Value{&value{c, kind, nil}}
	}
	return v
}

func makeInt(x interface{}) (c constant.Value, badkind bool) {
	var i int64
	var u uint64

	switch x := x.(type) {
	case int:
		i = int64(x)
	case int8:
		i = int64(x)
	case int16:
		i = int64(x)
	case int32:
		i = int64(x)
	case int64:
		i = x
	case uint:
		u = uint64(x)
	case uint8:
		u = uint64(x)
	case uint16:
		u = uint64(x)
	case uint32:
		u = uint64(x)
	case uint64:
		u = x
	case uintptr:
		u = uint64(x)
	default:
		return nil, badkind
	}
	if u != 0 {
		c = constant.MakeUint64(u)
	} else {
		c = constant.MakeInt64(i)
	}
	return c, false
}

/**
 * Create the complex constant i * c
 */
func MakeImag(v Value) Value {
	if v.value == nil {
		return invalid
	}
	switch c := v.cval; c.Kind() {
	case constant.Int, constant.Float:
		return Value{&value{constant.MakeImag(c), kind2complex(v.kind), v.err}}
	default:
		return errNotNumeric(c)
	}
}

// return numerator of Value v.
// v.Kind() must be Int*, Uint*, Float*, UntypedInt, UntypedRune, UntypedFloat or Invalid
func (v Value) Num() Value {
	if v.value == nil {
		return invalid
	}
	switch c := v.cval; c.Kind() {
	case constant.Int, constant.Float:
		return Value{&value{constant.Num(v.cval), kind2numerator(v.kind), v.err}}
	default:
		return errNotReal(c)
	}
}

// return denominator of Value.
// v.Kind() must be Int*, Uint*, Float*, UntypedInt, UntypedRune, UntypedFloat or Invalid
func (v Value) Den() Value {
	if v.value == nil {
		return invalid
	}
	switch c := v.cval; c.Kind() {
	case constant.Int, constant.Float:
		return Value{&value{constant.Denom(v.cval), kind2numerator(v.kind), v.err}}
	default:
		return errNotReal(c)
	}
}

// return real part of Value v.
// v.Kind() must be Int*, Uint*, Float*, Complex*,
// UntypedInt, UntypedRune, UntypedFloat, UntypedComplex or Invalid
func (v Value) Real() Value {
	if v.value == nil {
		return invalid
	}
	switch c := v.cval; c.Kind() {
	case constant.Int, constant.Float, constant.Complex:
		return Value{&value{constant.Real(v.cval), kind2real(v.kind), v.err}}
	default:
		return errNotNumeric(c)
	}
}

// return imaginary part of Value v.
// v.Kind() must be Int*, Uint*, Float*, Complex*,
// UntypedInt, UntypedRune, UntypedFloat, UntypedComplex or Invalid
func (v Value) Imag() Value {
	if v.value == nil {
		return invalid
	}
	switch c := v.cval; c.Kind() {
	case constant.Int, constant.Float, constant.Complex:
		return Value{&value{constant.Imag(v.cval), kind2real(v.kind), v.err}}
	default:
		return errNotNumeric(c)
	}
}

func kind2complex(kind Kind) Kind {
	if kind.IsUntyped() {
		kind = UntypedComplex
	} else if kind == Float32 {
		kind = Complex64
	} else {
		kind = Complex128
	}
	return kind
}

func kind2numerator(kind Kind) Kind {
	switch kind {
	case Float32, Float64:
		kind = Int64
	case UntypedFloat:
		kind = UntypedInt
	}
	return kind
}

func kind2real(kind Kind) Kind {
	switch kind {
	case Complex64:
		kind = Float32
	case Complex128:
		kind = Float64
	case UntypedComplex:
		kind = UntypedFloat
	}
	return kind
}

/**
 * try to convert Value v to to specified kind.
 * return Value{Invalid, ErrorBadKind} if conversion is not allowed.
 * return Value{Invalid, ErrorOverflow} if conversion is not exact.
 * otherwise return converted Value
 */
func (v Value) To(kind Kind) Value {
	if v.kind == kind {
		return v
	} else if kind == Invalid {
		return invalid
	}
	var c constant.Value
	switch kind {
	case Int, Int8, Int16, Int32, Int64,
		Uint, Uint8, Uint16, Uint32, Uint64, Uintptr,
		UntypedInt:
		c = constant.ToInt(v.cval)
	case Float32, Float64, UntypedFloat:
		c = constant.ToFloat(v.cval)
	case Complex64, Complex128, UntypedComplex:
		c = constant.ToComplex(v.cval)
	}
	return validate(c, kind)
}

func validate(c constant.Value, kind Kind) Value {
	badkind, exact := false, false

	ckind := c.Kind()
	if ckind == constant.Unknown {
		return invalid
	}
	switch kind {
	case Invalid:
		badkind = true
	case Bool, UntypedBool:
		badkind = ckind != constant.Bool
		exact = true
	case Int, Int8, Int16, Int32, Int64:
		if ckind == constant.Int {
			i, iexact := constant.Int64Val(c)
			exact = iexact && int64fits(i, kind)
		} else {
			badkind = true
		}
	case Uint, Uint8, Uint16, Uint32, Uint64, Uintptr:
		if ckind == constant.Int {
			n, nexact := constant.Uint64Val(c)
			exact = nexact && uint64fits(n, kind)
		} else {
			badkind = true
		}
	case Float32:
		if ckind == constant.Int || ckind == constant.Float {
			_, exact = constant.Float32Val(c)
		} else {
			badkind = true
		}
	case Float64:
		if ckind == constant.Int || ckind == constant.Float {
			_, exact = constant.Float64Val(c)
		} else {
			badkind = true
		}
	case Complex64:
		if ckind == constant.Int || ckind == constant.Float || ckind == constant.Complex {
			_, rexact := constant.Float32Val(constant.Real(c))
			_, iexact := constant.Float32Val(constant.Imag(c))
			exact = rexact && iexact
		} else {
			badkind = true
		}
	case Complex128:
		if ckind == constant.Int || ckind == constant.Float || ckind == constant.Complex {
			_, rexact := constant.Float64Val(constant.Real(c))
			_, iexact := constant.Float64Val(constant.Imag(c))
			exact = rexact && iexact
		} else {
			badkind = true
		}
	case String, UntypedString:
		badkind = ckind != constant.String
		exact = true
	case UntypedInt, UntypedRune:
		c = constant.ToInt(c)
		exact = true
	case UntypedFloat:
		c = constant.ToFloat(c)
		exact = true
	case UntypedComplex:
		c = constant.ToComplex(c)
		exact = true
	case UntypedNil:
		c = nil
		exact = true
	}

	v_ := *invalid.value
	if badkind || c.Kind() == constant.Unknown {
		v_.err = ErrorKind{c.String(), kind}
	} else if !exact {
		v_.err = ErrorOverflow{c, kind}
	} else {
		v_ = value{c, kind, nil}
	}
	return Value{&v_}
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

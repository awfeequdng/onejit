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

// wraps go/constant.Value
type Value struct {
	cval constant.Value
	kind Kind
}

var (
	invalid    = Value{constant.MakeUnknown(), Invalid}
	untypedNil = Value{constant.MakeUnknown(), UntypedNil}
)

func (v *Value) Kind() Kind {
	return v.kind
}

/** return a short, quoted, user-readable string representation of v */
func (v *Value) String() string {
	if v.kind == Invalid || v.kind == UntypedNil {
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
 * v.Kind() must be Int*, Uint*, UntypedInt, UntypedRune or Invalid
 */
func (v *Value) Int64() (int64, bool) {
	return constant.Int64Val(v.cval)
}

/**
 * return the int64 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, UntypedInt, UntypedRune or Invalid
 */
func (v *Value) Uint64() (uint64, bool) {
	return constant.Uint64Val(v.cval)
}

/**
 * return the float32 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, Float*, UntypedInt, UntypedRune, UntypedFloat or Invalid
 */
func (v *Value) Float32() (float32, bool) {
	return constant.Float32Val(v.cval)
}

/**
 * return the float32 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, Float*, UntypedInt, UntypedRune, UntypedFloat or Invalid
 */
func (v *Value) Float64() (float64, bool) {
	return constant.Float64Val(v.cval)
}

/**
 * return the complex64 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, Float*, Complex*,
 * UntypedInt, UntypedRune, UntypedFloat, UntypedComplex or Invalid
 */
func (v *Value) Complex64() (complex64, bool) {
	re, rexact := v.Real().Float32()
	im, iexact := v.Imag().Float32()
	return complex(re, im), rexact && iexact
}

/**
 * return the complex128 value of v and whether the result is exact;
 * v.Kind() must be Int*, Uint*, Float*, Complex*,
 * UntypedInt, UntypedRune, UntypedFloat, UntypedComplex or Invalid
 */
func (v *Value) Complex128() (complex128, bool) {
	re, rexact := v.Real().Float64()
	im, iexact := v.Imag().Float64()
	return complex(re, im), rexact && iexact
}

/**
 * return the string value of v;
 * v.Kind() must be String, UntypedString or Invalid
 */
func (v *Value) StringVal() string {
	return constant.StringVal(v.cval)
}

/**
 * Create a constant. Allowed val types depend on kind:
 * bool    if Kind is Bool or UntypedBool
 * string  if Kind is String or UntypedString
 * int64, uint64 or *math/big.Int if kind is Int*, Uint*, UntypedInt or UntypedRune
 * float64, *big.Float or *big.Rat if kind is Float* or UntypedFloat
 * complex128 if kind is Complex* or UntypedComplex
 * nil     if Kind is Invalid or UntypedNil
 *
 * return &Value{Invalid}, ErrKind if conversion of val to kind is not allowed.
 * return &Value{Invalid}, ErrOverflow if conversion of val to kind is not exact.
 * otherwise return value, nil
 */
func Make(kind Kind, x interface{}) (*Value, error) {
	var c constant.Value
	needvalidate := !kind.IsUntyped()
	badkind := false

	switch kind {
	case Invalid:
		if x == nil {
			return &invalid, nil
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

		switch x := x.(type) {
		case int64:
			c = constant.MakeInt64(x)
		case uint64:
			c = constant.MakeUint64(x)
		case *big.Int:
			c = constant.Make(x)
		default:
			badkind = true
		}
	case Float32, Float64, UntypedFloat:
		switch x := x.(type) {
		case float64:
			c = constant.MakeFloat64(x)
		case *big.Rat, *big.Float:
			c = constant.Make(x)
		default:
			badkind = true
		}
	case Complex64, Complex128, UntypedComplex:
		cplx, ok := x.(complex128)
		if !ok {
			badkind = true
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
			return &untypedNil, nil
		}
		badkind = true
	default:
		badkind = true
	}
	v := &invalid
	var err error
	if badkind {
		err = ErrorKind{fmt.Sprint(x), kind}
	} else if needvalidate {
		v, err = validate(c, kind)
	} else {
		v = &Value{c, kind}
	}
	return v, err
}

/**
 * Create the complex constant i * c
 */
func MakeImag(v *Value) *Value {
	return &Value{constant.MakeImag(v.cval), kind2complex(v.kind)}
}

// return numerator of Value v.
// v.Kind() must be Int*, Uint*, Float*, UntypedInt, UntypedRune, UntypedFloat or Invalid
func (v *Value) Num() *Value {
	return &Value{constant.Num(v.cval), kind2numerator(v.kind)}
}

// return denominator of Value.
// v.Kind() must be Int*, Uint*, Float*, UntypedInt, UntypedRune, UntypedFloat or Invalid
func (v *Value) Den() *Value {
	return &Value{constant.Denom(v.cval), kind2numerator(v.kind)}
}

// return real part of Value v.
// v.Kind() must be Int*, Uint*, Float*, Complex*,
// UntypedInt, UntypedRune, UntypedFloat, UntypedComplex or Invalid
func (v *Value) Real() *Value {
	return &Value{constant.Real(v.cval), kind2real(v.kind)}
}

// return imaginary part of Value v.
// v.Kind() must be Int*, Uint*, Float*, Complex*,
// UntypedInt, UntypedRune, UntypedFloat, UntypedComplex or Invalid
func (v *Value) Imag() *Value {
	return &Value{constant.Imag(v.cval), kind2real(v.kind)}
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
 * return &Value{Invalid}, ErrorBadKind if conversion is not allowed.
 * return &Value{Invalid}, ErrorOverflow if conversion is not exact.
 * otherwise return Value and nil
 */
func (v *Value) To(kind Kind) (*Value, error) {
	if v.kind == kind {
		return v, nil
	} else if kind == Invalid {
		return &invalid, nil
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

func validate(c constant.Value, kind Kind) (*Value, error) {
	badkind, exact := false, false

	ckind := c.Kind()
	if ckind == constant.Unknown {
		return nil, ErrInvalid
	}
	switch kind {
	case Invalid:
		badkind = true
	case Bool, UntypedBool:
		badkind = ckind != constant.Bool
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

	v := &invalid
	var err error
	if badkind || c.Kind() == constant.Unknown {
		err = ErrorKind{c.String(), kind}
	} else if !exact {
		err = ErrorOverflow{c, kind}
	} else {
		v = &Value{c, kind}
	}
	return v, err
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

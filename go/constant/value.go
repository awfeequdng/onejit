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
	gotoken "go/token"
	"math/big"

	"github.com/cosmos72/onejit/go/token"
	"github.com/cosmos72/onejit/go/types"
)

type value struct {
	cval constant.Value
	typ  *types.Complete
	err  error
}

// equivalent to go/constant.Value
// the zero value i.e. Value{} has .Type() == nil, .Kind() = Invalid and .Err() = ErrInvalid
type Value struct {
	*value
}

var (
	cunknown = constant.MakeUnknown()

	invalid = Value{&value{cunknown, nil, ErrInvalid}}
	// types.BasicType(UntypedBool) does not depend on config.Target()
	untypedNil = Value{&value{cunknown, types.BasicType(UntypedBool), nil}}
)

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
 * IsValid returns true if kind is != Invalid.
 */
func (v Value) IsValid() bool {
	return v.value != nil && v.typ != nil
}

func (v Value) Kind() Kind {
	if !v.IsValid() {
		return Invalid
	}
	return v.typ.Kind()
}

func (v Value) Type() *types.Complete {
	if !v.IsValid() {
		return nil
	}
	return v.typ
}

// if v is typed, return Type()
// if v is untyped, return its default type
func (v Value) DefaultType() *types.Complete {
	var kind Kind
	switch v.Kind() {
	case UntypedBool:
		kind = Bool
	case UntypedInt:
		kind = Int
	case UntypedRune:
		kind = Rune
	case UntypedFloat:
		kind = Float32
	case UntypedComplex:
		kind = Complex128
	case UntypedString:
		kind = String
	case UntypedNil:
		kind = Invalid
	default:
		return v.Type()
	}
	return types.BasicType(kind)
}

/** return a short, quoted, user-readable string representation of v */
func (v Value) String() string {
	if v.value == nil {
		return ErrInvalid.Error()
	} else if v.err != nil {
		return "{error: " + v.err.Error() + "}"
	} else if v.typ == nil {
		return ErrInvalid.Error()
	}
	c := v.cval
	kind := v.typ.Kind()
	if c == nil {
		return kind.String()
	} else if kind == UntypedRune {
		return "{" + kind.String() + " " + untypedRune2string(c) + "}"
	} else if kind.IsUntyped() {
		return "{" + kind.String() + " " + c.String() + "}"
	} else {
		return c.String()
	}
}

/**
 * get the value of a constant. returned type is:
 * bool    if Kind is Bool
 * string  if Kind is String
 * int64 or *math/big.Int if Kind is Int*, Uint*, UntypedInt or UntypedRune
 * *big.Float or *big.Rat if Kind is Float* or UntypedFloat
 * nil     if Kind is Invalid, Complex*, UntypedComplex or UntypedNil
 */
func (v Value) Value() interface{} {
	if v.value == nil {
		return nil
	}
	return constant.Val(v.cval)
}

/**
 * Bool returns the bool value of v.
 * v.Kind() must be Bool or Invalid
 */
func (v Value) Bool() bool {
	if !v.IsValid() {
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
	if v.value == nil || v.cval == nil || v.cval.Kind() != constant.String {
		return "", false
	}
	return constant.StringVal(v.cval), true
}

/**
 * MakeKind creates a constant.
 * It is a shortcut for Make(types.BasicType(kind), x)
 */
func MakeKind(kind Kind, x interface{}) Value {
	return Make(types.BasicType(kind), x)
}

/**
 * Make creates a constant. Allowed x types depend on t.Kind():
 * bool    if kind is Bool or UntypedBool
 * string  if kind is String or UntypedString
 * int*, uint* or *math/big.Int if kind is Int*, Uint*, UntypedInt or UntypedRune
 * float32, float64, *big.Float or *big.Rat if kind is Float* or UntypedFloat
 * complex64 or complex128 if kind is Complex* or UntypedComplex
 * nil     if kind is Invalid or UntypedNil
 *
 * return Value{Invalid, ErrorType} if conversion of val to kind is not allowed.
 * return Value{Invalid, ErrorOverflow} if conversion of val to kind is not exact.
 */
func Make(t *types.Complete, x interface{}) Value {
	if t == nil {
		return invalid
	}
	var c constant.Value
	kind := t.Kind()
	needvalidate := !kind.IsUntyped()
	badkind := false

	switch kind.Category() {
	case Bool:
		switch x := x.(type) {
		case bool:
			c = constant.MakeBool(x)
			needvalidate = false
		default:
			badkind = true
		}
	case Int:
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
		c = constant.BinaryOp(cre, gotoken.ADD, cim)

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
		v = Value{&value{cunknown, nil, ErrorType{fmt.Sprint(x), t}}}
	} else if needvalidate {
		v = validate(c, t)
	} else {
		v = Value{&value{c, t, nil}}
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
		return Value{&value{constant.MakeImag(c), type2complex(v.typ), v.err}}
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
		return Value{&value{constant.Num(v.cval), type2numerator(v.typ), v.err}}
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
		return Value{&value{constant.Denom(v.cval), type2numerator(v.typ), v.err}}
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
		return Value{&value{constant.Real(v.cval), type2real(v.typ), v.err}}
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
		return Value{&value{constant.Imag(v.cval), type2real(v.typ), v.err}}
	default:
		return errNotNumeric(c)
	}
}

func type2complex(t *types.Complete) *types.Complete {
	if t == nil {
		return nil
	}
	kind := t.Kind()
	if kind.IsUntyped() {
		t = types.BasicType(UntypedComplex)
	} else if kind == Float32 {
		t = types.BasicType(Complex64)
	} else {
		t = types.BasicType(Complex128)
	}
	return t
}

func type2numerator(t *types.Complete) *types.Complete {
	if t == nil {
		return nil
	}
	kind := t.Kind()
	switch kind {
	case Float32, Float64:
		t = types.BasicType(Int64)
	case UntypedFloat:
		t = types.BasicType(UntypedInt)
	}
	return t
}

func type2real(t *types.Complete) *types.Complete {
	if t == nil {
		return nil
	}
	kind := t.Kind()
	switch kind {
	case Complex64:
		t = types.BasicType(Float32)
	case Complex128:
		t = types.BasicType(Float64)
	case UntypedComplex:
		t = types.BasicType(UntypedFloat)
	}
	return t
}

/**
 * try to convert Value v to to specified type.
 * return Value{Invalid, ErrorBadKind} if conversion is not allowed.
 * return Value{Invalid, ErrorOverflow} if conversion is not exact.
 * otherwise return converted Value
 */
func (v Value) To(t *types.Complete) Value {
	if !v.IsValid() || t == nil {
		return invalid
	} else if v.typ == t {
		return v
	} else if v.typ.Kind() == t.Kind() {
		// changing the type, but not its kind
		return Value{&value{v.cval, v.typ, v.err}}
	}
	var c constant.Value
	switch t.Kind().Category() {
	case Int:
		c = constant.ToInt(v.cval)
	case Float64:
		c = constant.ToFloat(v.cval)
	case Complex64, Complex128, UntypedComplex:
		c = constant.ToComplex(v.cval)
	}
	return validate(c, t)
}

/**
 * try to convert Value v to to specified kind.
 * return Value{Invalid, ErrorBadKind} if conversion is not allowed.
 * return Value{Invalid, ErrorOverflow} if conversion is not exact.
 * otherwise return converted Value
 */
func (v Value) ToKind(kind Kind) Value {
	return v.To(types.BasicType(kind))
}

func validate(c constant.Value, t *types.Complete) Value {
	badkind, exact := false, false

	if c == nil || t == nil || c.Kind() == constant.Unknown {
		return invalid
	}
	ckind := c.Kind()
	kind := t.Kind()
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
		v_.err = ErrorType{c.String(), t}
	} else if !exact {
		v_.err = ErrorOverflow{c, kind}
	} else {
		v_ = value{c, t, nil}
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

func MakeZero(t *types.Complete) Value {
	if t == nil {
		return invalid
	}
	var c constant.Value
	var err error
	switch t.Kind().Category() {
	case Bool:
		c = constant.MakeBool(false)
	case Int:
		c = constant.MakeInt64(0)
	case Float64:
		c = constant.MakeFloat64(0.0)
	case Complex128:
		c = constant.MakeImag(constant.MakeFloat64(0.0))
	case String:
		c = constant.MakeString("")
	default:
		c = cunknown
		err = ErrorType{"constant.MakeZero(): unsupported type " + t.String(), nil}
	}
	return Value{&value{c, t, err}}
}

func MakeFromLiteral(lit string, tok token.Token) Value {
	c := constant.MakeFromLiteral(lit, gotoken.Token(tok), 0)
	if c.Kind() == constant.Unknown {
		return invalid
	}
	kind := Invalid
	switch tok {
	case token.INT:
		kind = UntypedInt
	case token.FLOAT:
		kind = UntypedFloat
	case token.IMAG:
		kind = UntypedComplex
	case token.CHAR:
		kind = UntypedRune
	case token.STRING:
		kind = UntypedString
	}
	return Value{&value{c, types.BasicType(kind), nil}}
}

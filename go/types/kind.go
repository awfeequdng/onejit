/*
 * Copyright (C 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * kind.go
 *
 *  Created on: Mar 31, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import "github.com/cosmos72/onejit/go/token"

type Kind = token.Kind

const (
	Invalid = token.Invalid

	Bool          = token.Bool
	Int           = token.Int
	Int8          = token.Int8
	Int16         = token.Int16
	Int32         = token.Int32
	Int64         = token.Int64
	Uint          = token.Uint
	Uint8         = token.Uint8
	Uint16        = token.Uint16
	Uint32        = token.Uint32
	Uint64        = token.Uint64
	Uintptr       = token.Uintptr
	Float32       = token.Float32
	Float64       = token.Float64
	Complex64     = token.Complex64
	Complex128    = token.Complex128
	ArrayKind     = token.ArrayKind
	ChanKind      = token.ChanKind
	FuncKind      = token.FuncKind
	InterfaceKind = token.InterfaceKind
	MapKind       = token.MapKind
	PtrKind       = token.PtrKind
	SliceKind     = token.SliceKind
	String        = token.String
	StructKind    = token.StructKind
	UnsafePointer = token.UnsafePointer

	// types for untyped values
	UntypedBool    = token.UntypedBool
	UntypedInt     = token.UntypedInt
	UntypedRune    = token.UntypedRune
	UntypedFloat   = token.UntypedFloat
	UntypedComplex = token.UntypedComplex
	UntypedString  = token.UntypedString
	UntypedNil     = token.UntypedNil

	// aliases
	Byte = token.Byte
	Rune = token.Rune
)

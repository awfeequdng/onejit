/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * kind.go
 *
 *  Created on: Apr 12, 2021
 *      Author: Massimiliano Ghilardi
 */

package constant

import "github.com/cosmos72/onejit/go/token"

// alias for token.Kind
type Kind = token.Kind

const (
	Invalid = Kind(token.Invalid)

	Bool          = Kind(token.Bool)
	Int           = Kind(token.Int)
	Int8          = Kind(token.Int8)
	Int16         = Kind(token.Int16)
	Int32         = Kind(token.Int32)
	Int64         = Kind(token.Int64)
	Uint          = Kind(token.Uint)
	Uint8         = Kind(token.Uint8)
	Uint16        = Kind(token.Uint16)
	Uint32        = Kind(token.Uint32)
	Uint64        = Kind(token.Uint64)
	Uintptr       = Kind(token.Uintptr)
	Float32       = Kind(token.Float32)
	Float64       = Kind(token.Float64)
	Complex64     = Kind(token.Complex64)
	Complex128    = Kind(token.Complex128)
	ArrayKind     = Kind(token.ArrayKind)
	ChanKind      = Kind(token.ChanKind)
	FuncKind      = Kind(token.FuncKind)
	InterfaceKind = Kind(token.InterfaceKind)
	MapKind       = Kind(token.MapKind)
	PtrKind       = Kind(token.PtrKind)
	SliceKind     = Kind(token.SliceKind)
	String        = Kind(token.String)
	StructKind    = Kind(token.StructKind)
	UnsafePointer = Kind(token.UnsafePointer)

	// types for untyped values
	UntypedBool    = Kind(token.UntypedBool)
	UntypedInt     = Kind(token.UntypedInt)
	UntypedRune    = Kind(token.UntypedRune)
	UntypedFloat   = Kind(token.UntypedFloat)
	UntypedComplex = Kind(token.UntypedComplex)
	UntypedString  = Kind(token.UntypedString)
	UntypedNil     = Kind(token.UntypedNil)

	// aliases
	Byte = Kind(token.Byte)
	Rune = Kind(token.Rune)
)

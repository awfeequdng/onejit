/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * compare.go
 *
 *  Created on: Mar 31, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

func IdenticalType(c1 *Complete, c2 *Complete, ignoreTags bool) bool {
	if c1 == nil || c2 == nil || c1.hash != c2.hash {
		return false
	} else if c1 == c2 {
		return true
	} else if !ignoreTags {
		// types are canonicalized
		return false
	}
	cmp := comparator{
		cache:      nil,
		ignoreTags: ignoreTags,
	}
	return cmp.identicalType(c1, c2)
}

type comparator struct {
	cache      map[*Complete]map[*Complete]struct{}
	ignoreTags bool
}

func (cmp *comparator) add(c1 *Complete, c2 *Complete) {
	cache := cmp.cache
	if cache == nil {
		cache = make(map[*Complete]map[*Complete]struct{})
		cmp.cache = cache
	}
	set := cache[c1]
	if set == nil {
		set = make(map[*Complete]struct{})
		cache[c1] = set
	}
	set[c2] = struct{}{}
}

func (cmp *comparator) del(c1 *Complete, c2 *Complete) {
	delete(cmp.cache[c1], c2)
}

func (cmp *comparator) identicalType(c1 *Complete, c2 *Complete) bool {
	if c1 == nil || c2 == nil || c1.hash != c2.hash {
		return false
	} else if c1 == c2 {
		return true
	}
	k1, k2 := c1.kind, c2.kind
	if k1 != k2 {
		return false
	}
	if k1 < ArrayKind || k1 > StructKind {
		return c1 == c2
	}
	named1, named2 := c1.Name() != "", c2.Name() != ""
	if named1 || named2 {
		// at least one type is named => they cannot be identical
		return false
	}
	if _, cached := cmp.cache[c1][c2]; cached {
		// already comparing these two recursive types
		return true
	}
	cmp.add(c1, c2)
	var ok bool
	switch k1 {
	case ArrayKind:
		ok = c1.Len() == c2.Len() && cmp.identicalType(c1.Elem(), c2.Elem())
	case ChanKind:
		ok = c1.ChanDir() == c2.ChanDir() && cmp.identicalType(c1.Elem(), c2.Elem())
	case FuncKind:
		ok = cmp.identicalFunc(c1, c2)
	case MapKind:
		ok = cmp.identicalType(c1.Key(), c2.Key()) && cmp.identicalType(c1.Elem(), c2.Elem())
	case InterfaceKind:
		ok = cmp.identicalInterface(c1, c2)
	case PtrKind, SliceKind:
		ok = cmp.identicalType(c1.Elem(), c2.Elem())
	case StructKind:
		ok = cmp.identicalStruct(c1, c2)
	}
	cmp.del(c1, c2)
	return ok
}

func (cmp *comparator) identicalFunc(c1 *Complete, c2 *Complete) bool {
	return c1.NumIn() == c2.NumIn() && c1.NumOut() == c2.NumOut() &&
		c1.IsVariadic() == c2.IsVariadic() &&
		cmp.identicalTypes(c1.extra.types, c2.extra.types)
}

func (cmp *comparator) identicalInterface(c1 *Complete, c2 *Complete) bool {
	return cmp.IdenticalMethods(c1.extra.methods, c2.extra.methods)
}

func (cmp *comparator) identicalStruct(c1 *Complete, c2 *Complete) bool {
	return cmp.identicalFields(c1.extra.fields, c2.extra.fields)
}

func (cmp *comparator) identicalTypes(ts1 []Type, ts2 []Type) bool {
	if len(ts1) != len(ts2) {
		return false
	}
	for i := range ts1 {
		if !cmp.identicalType(ts1[i].common(), ts2[i].common()) {
			return false
		}
	}
	return true
}

func (cmp *comparator) identicalField(f1 *Field, f2 *Field) bool {
	if f1 == f2 {
		return true
	}
	return f1.Name == f2.Name && f1.PkgPath == f2.PkgPath &&
		(cmp.ignoreTags || f1.Tag == f2.Tag) &&
		cmp.identicalType(f1.Type.common(), f2.Type.common())
}

func (cmp *comparator) identicalFields(fs1 []Field, fs2 []Field) bool {
	if len(fs1) != len(fs2) {
		return false
	}
	for i := range fs1 {
		if !cmp.identicalField(&fs1[i], &fs2[i]) {
			return false
		}
	}
	return true
}

func (cmp *comparator) identicalMethod(m1 *Method, m2 *Method) bool {
	if m1 == m2 {
		return true
	}
	return m1.Name == m2.Name && m1.PkgPath == m2.PkgPath &&
		cmp.identicalType(m1.Type.common(), m2.Type.common())
}

func (cmp *comparator) IdenticalMethods(ms1 []Method, ms2 []Method) bool {
	if len(ms1) != len(ms2) {
		return false
	}
	for i := range ms1 {
		if !cmp.identicalMethod(&ms1[i], &ms2[i]) {
			return false
		}
	}
	return true
}

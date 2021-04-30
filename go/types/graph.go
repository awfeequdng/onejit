/*
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * graph.go
 *
 *  Created on: Mar 31, 2021
 *      Author: Massimiliano Ghilardi
 */

package types

import "github.com/cosmos72/onejit/go/io"

type (
	typelist []Type
	typeset  map[Type]struct{}

	typedeps struct {
		fstrong typeset
		fweak   typeset
	}
	// map Type -> types it depends from
	typegraph map[Type]*typedeps

	typegraph2 struct {
		fwd, inv  typegraph // forward and invertse dependencies
		completed typelist
	}
)

const debugComplete = false

// resolve circular dependencies and incomplete Types,
// and return equivalent *Complete objects
func CompleteTypes(ts ...Type) []*Complete {
	if !allTypesAreComplete(ts) {
		fwd := make(typegraph, len(ts))
		fwd.addTypes(ts)
		g2 := typegraph2{
			fwd:       fwd,
			inv:       fwd.invert(),
			completed: make(typelist, 0, len(ts)),
		}
		g2.complete()
	}
	cs := make([]*Complete, len(ts))
	for i, t := range ts {
		cs[i] = t.common()
	}
	return cs
}

func allTypesAreComplete(ts []Type) bool {
	for _, t := range ts {
		if t.common().flags&flagComplete == 0 {
			return false
		}
	}
	return true
}

// --------------------------- typedeps ----------------------------------------

func (d *typedeps) strong() typeset {
	if d == nil {
		return nil
	}
	return d.fstrong
}

func (d *typedeps) weak() typeset {
	if d == nil {
		return nil
	}
	return d.fweak
}

// --------------------------- typegraph ---------------------------------------

func (g typegraph) addTypes(ts []Type) {
	for _, t := range ts {
		g.addType(t)
	}
}

func (g typegraph) addType(t Type) bool {
	if t == nil || t.common().flags&flagComplete != 0 {
		return false
	}
	if g[t] == nil {
		g[t] = &typedeps{} // mark t as added: avoids infinite recursion
		g.addTypeContent(t)
	}
	return true
}

func (g typegraph) addTypeContent(t Type) {
	if debugComplete {
		println("addTypeContent", t.String())
	}
	c := t.common()
	x := c.extra
	if _, isnamed := t.(*Named); isnamed {
		u := x.underlying
		if u == nil {
			panic("CompleteTypes: named type " + t.String() + " has nil underlying type")
		}
		// named type has a strong dependency from its underlying type,
		// every other dependency derives from it
		g.addDepType(t, u, true)
		return
	}
	// do not collect c.ptrTo: it's not a dependency

	if c.elem != nil {
		g.addDepType(t, c.elem, c.kind == ArrayKind)
	}
	if x == nil {
		return
	}
	g.addDepTypes(t, x.types, c.kind == InterfaceKind)
	g.addDepFields(t, x.fields)
	if c.kind == InterfaceKind {
		g.addDepMethods(t, x.methods)
	}
}

func (g typegraph) addDepTypes(t Type, tdeps []Type, strong bool) {
	for _, tdep := range tdeps {
		g.addDepType(t, tdep, strong)
	}
}

func (g typegraph) addDepFields(t Type, fdeps []Field) {
	for i := range fdeps {
		g.addDepType(t, fdeps[i].Type, true)
	}
}

func (g typegraph) addDepMethods(t Type, mdeps []Method) {
	for i := range mdeps {
		g.addDepType(t, mdeps[i].Type, false)
	}
}

func (g typegraph) addDepType(t Type, tdep Type, strong bool) {
	if g.addType(tdep) {
		g.add(t, tdep, strong)
	}
}

func (g typegraph) add(t Type, tdep Type, strong bool) {
	deps := g[t]
	if deps == nil {
		deps = &typedeps{}
		g[t] = deps
	}
	if strong {
		switch tdep.common().kind {
		case ChanKind, FuncKind, InterfaceKind, MapKind, PtrKind, SliceKind:
			// if tdep is unnamed, strong dependency from one of these types is actually weak
			_, isnamed := tdep.(*Named)
			strong = isnamed
		}
	}
	if strong {
		if deps.fstrong == nil {
			deps.fstrong = make(typeset)
		}
		deps.fstrong[tdep] = struct{}{}
		delete(deps.fweak, tdep)
	} else if _, haveStrong := deps.fstrong[tdep]; !haveStrong {
		if deps.fweak == nil {
			deps.fweak = make(typeset)
		}
		deps.fweak[tdep] = struct{}{}
	}
}

func (g typegraph) del(t Type, tdep Type, strong bool) {
	if deps := g[t]; deps != nil {
		if strong {
			delete(deps.fstrong, tdep)
		}
		delete(deps.fweak, tdep)
	}
}

func (g typegraph) invert() typegraph {
	invg := make(typegraph, len(g))
	for t, deps := range g {
		for tdep := range deps.strong() {
			invg.add(tdep, t, true)
		}
		for tdep := range deps.weak() {
			invg.add(tdep, t, false)
		}
	}
	return invg
}

func (g typegraph) pickTypeNoStrongDeps() Type {
	var first Type
	for t, deps := range g {
		if len(deps.strong()) == 0 {
			return t
		} else if first == nil {
			first = t
		}
	}
	panic("CompleteTypes: invalid recursive type " + first.String())
}

// --------------------------- typegraph2 --------------------------------------

func (g2 *typegraph2) complete() {
	fwd := g2.fwd
	for len(fwd) != 0 {
		if debugComplete {
			println(g2.String())
		}
		t := fwd.pickTypeNoStrongDeps()
		if debugComplete {
			println("completeType", t.String())
		}
		t.complete()
		g2.updateDeps(t)
	}
	if debugComplete {
		println(g2.String())
	}
	for _, t := range g2.completed {
		checkCompleteType(t)
		t.common().flags |= flagComplete
	}
}

func checkCompleteType(t Type) {
	c := t.common()
	isComparable := c.flags&flagComparable != 0
	isNotComparable := c.flags&flagNotComparable != 0
	if isComparable == isNotComparable {
		panic("CompleteTypes: type " + t.String() + " is both comparable and not comparable")
	}
	if c.kind == MapKind {
		if c.extra.types[0].common().flags&(flagComparable|flagNotComparable) != flagComparable {
			panic("CompleteTypes: map key type " + t.String() + " is not comparable")
		}
	}
}

func (g2 *typegraph2) updateDeps(completed Type) {
	t := completed
	for tdep := range g2.fwd[t].strong() {
		g2.inv.del(tdep, t, true)
	}
	for tdep := range g2.fwd[t].weak() {
		g2.inv.del(tdep, t, false)
	}
	for tdep := range g2.inv[t].strong() {
		g2.fwd.del(tdep, t, true)
	}
	for tdep := range g2.inv[t].weak() {
		g2.fwd.del(tdep, t, false)
	}
	delete(g2.fwd, t)
	delete(g2.inv, t)
	g2.completed = append(g2.completed, t)
}

// --------------------------- String, WriteTo ---------------------------------

func (g2 *typegraph2) String() string {
	var b builder
	g2.WriteTo(&b, fullPkgPath)
	return b.String()
}

func (g2 *typegraph2) WriteTo(dst io.StringWriter, flag verbose) {
	dst.WriteString("---------------------\n")
	g2.fwd.WriteTo(dst, "FORWARD", flag)
	dst.WriteString("\n")
	g2.inv.WriteTo(dst, "INVERSE", flag)
	dst.WriteString("\n")
	g2.completed.WriteTo(dst, "COMPLETED", flag)
}

func (g typegraph) WriteTo(dst io.StringWriter, label string, flag verbose) {
	dst.WriteString("(")
	dst.WriteString(label)
	for t, deps := range g {
		dst.WriteString("\n    ")
		t.WriteTo(dst, flag)
		deps.WriteTo(dst, flag)
	}
	dst.WriteString("\n)")
}

func (d *typedeps) WriteTo(dst io.StringWriter, flag verbose) {
	if d == nil {
		dst.WriteString("nil")
		return
	}
	if d.fstrong != nil {
		dst.WriteString("\n        ")
		d.fstrong.WriteTo(dst, "STRONG", flag)
	}
	if d.fweak != nil {
		dst.WriteString("\n        ")
		d.fweak.WriteTo(dst, "WEAK", flag)
	}
}

func (s typeset) WriteTo(dst io.StringWriter, label string, flag verbose) {
	dst.WriteString("(")
	dst.WriteString(label)
	dst.WriteString(" ")
	i := 0
	for t := range s {
		if i != 0 {
			dst.WriteString("; ")
		}
		t.WriteTo(dst, flag)
		i++
	}
	dst.WriteString(")")
}

func (l typelist) WriteTo(dst io.StringWriter, label string, flag verbose) {
	dst.WriteString("(")
	dst.WriteString(label)
	dst.WriteString(" ")
	i := 0
	for _, t := range l {
		if i != 0 {
			dst.WriteString("; ")
		}
		t.WriteTo(dst, flag)
		i++
	}
	dst.WriteString(")")
}

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

import "strings"

type (
	typeset map[Type]struct{}

	typedeps struct {
		strong typeset
		weak   typeset
	}
	// map Type -> types it depends from
	typegraph map[Type]*typedeps
)

// resolve circular dependencies and incomplete Types,
// and return equivalent *Complete objects
func CompleteTypes(ts ...Type) []*Complete {
	g := make(typegraph, len(ts))
	g.addTypes(ts)
	println(g.String())
	g.complete()
	cs := make([]*Complete, len(ts))
	for i, t := range ts {
		cs[i] = t.common()
	}
	return cs
}

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
		g[t] = &typedeps{}
		g.addTypeContent(t)
	}
	return true
}

func (g typegraph) addTypeContent(t Type) {
	println("addTypeContent ", t.String())
	c := t.common()
	x := c.extra
	if _, isnamed := t.(*Named); isnamed {
		u := x.underlying
		if u == nil {
			panic("CompleteTypes named type " + t.String() + " has nil underlying type")
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
		deps := g[t]
		deps.add(g, t, tdep, strong)
	}
}

func (d *typedeps) add(g typegraph, t Type, tdep Type, strong bool) {
	if strong {
		switch tdep.common().kind {
		case ChanKind, FuncKind, InterfaceKind, MapKind, PtrKind, SliceKind:
			// if tdep is unnamed, strong dependency from one of these types is actually weak
			_, isnamed := tdep.(*Named)
			strong = isnamed
		}
	}
	if strong {
		if d.strong == nil {
			d.strong = make(typeset)
		}
		d.strong[tdep] = struct{}{}
		delete(d.weak, tdep)
	} else {
		if d.weak == nil {
			d.weak = make(typeset)
		}
		d.weak[tdep] = struct{}{}
	}
}

func (g typegraph) complete() {
	for t := range g {
		g.completeType(t)
	}
}

func (g typegraph) completeType(t Type) {
	c := t.common()
	c.flags |= flagComplete
	if c.flags&flagComparable == 0 {
		c.flags |= flagNotComparable
	}
	if named, _ := t.(*Named); named != nil {
		completeNamedUnderlying(named)
	}
}

func (g typegraph) String() string {
	var b strings.Builder
	g.writeTo(&b)
	return b.String()
}

func (g typegraph) writeTo(b *strings.Builder) {
	b.WriteString("(Graph")
	for t, deps := range g {
		b.WriteString("\n    ")
		b.WriteString(t.String())
		deps.writeTo(b)
	}
	b.WriteString("\n)")
}

func (d *typedeps) writeTo(b *strings.Builder) {
	if d == nil {
		b.WriteString("nil")
		return
	}
	if d.strong != nil {
		d.strong.writeTo(b, "(Strong ", ")")
	}
	if d.weak != nil {
		d.weak.writeTo(b, "(Weak ", ")")
	}
}

func (s typeset) writeTo(b *strings.Builder, enter string, leave string) {
	b.WriteString("\n        ")
	b.WriteString(enter)
	i := 0
	for t := range s {
		if i != 0 {
			b.WriteString("; ")
		}
		b.WriteString(t.String())
		i++
	}
	b.WriteString(")")
}

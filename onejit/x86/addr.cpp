/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * addr.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/const.hpp>
#include <onejit/x86/addr.hpp>

namespace onejit {
namespace x86 {

// ============================  Addr  ====================================

Node Addr::create(Func &func, Kind kind, const Label &label, const int32_t offset, const Var &base,
                  const Var &index, Scale scale) noexcept {
  Const coffset{func, offset};
  if (coffset) {
    Node nodes[] = {label, coffset, base, index};
    size_t len = 3;
    if (scale && index) {
      len = 4;
    } else {
      scale = Scale0;
    }
    return Tuple::create(func, kind, scale.opn(), Nodes{nodes, len});
  }
  return Addr{};
}

// shortcut for child(0).is<Label>()
Label Addr::label() const noexcept {
  return child(0).is<Label>();
}

// shortcut for child(1).is<Const>().imm().int32()
int32_t Addr::offset() const noexcept {
  return child(1).is<Const>().imm().int32();
}

// shortcut for child(2).is<Var>().local()
Local Addr::base() const noexcept {
  return child(2).is<Var>().local();
}

// shortcut for child(3).is<Var>().local()
Local Addr::index() const noexcept {
  return child(3).is<Var>().local();
}

////////////////////////////////////////////////////////////////////////////////
const Fmt &Addr::format_body(const Fmt &out) const {
  if (Label label = this->label()) {
    out << ' ' << label;
  }
  if (int32_t offset = this->offset()) {
    out << ' ' << offset << '_' << Int32.stringsuffix();
  }
  if (Local base = this->base()) {
    out << ' ' << base;
  }
  if (Local index = this->index()) {
    Scale scale = this->scale();
    if (scale != Scale0) {
      out << ' ' << index << " * " << scale;
    }
  }
  return out;
}

const Fmt &operator<<(const Fmt &out, const Addr &addr) {
  out << '(' << X86_ADDR;
  addr.format_body(out);
  return out << ')';
}

static const Fmt &format_addr(const Fmt &out, const onejit::Tuple &expr) {
  return out << expr.is<Addr>();
}

static const bool registered = (Tuple::register_formatter(X86_ADDR, format_addr),
                                Tuple::register_formatter(X86_ADDR_SCALE1, format_addr),
                                Tuple::register_formatter(X86_ADDR_SCALE2, format_addr),
                                Tuple::register_formatter(X86_ADDR_SCALE4, format_addr),
                                Tuple::register_formatter(X86_ADDR_SCALE8, format_addr));

} // namespace x86
} // namespace onejit

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
 * func.cpp
 *
 *  Created on Jan 13, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/imm.hpp>

namespace onejit {

Func::Func() noexcept //
    : Base{}, holder_{}, body_var_n_{}, vars_{}, labels_{}, body_{} {
}

Func &Func::reset(Code *holder, Name name, FuncType ftype) noexcept {
  holder_ = holder;
  body_var_n_ = 0;
  compiled_var_n_ = 0;
  vars_.clear();
  labels_.clear();
  body_ = Node{};
  for (size_t i = 0; i < ARCHID_N; i++) {
    compiled_[i] = Node{};
  }

  bool ok = bool(*this);
  for (size_t i = 0, n = ftype.param_n(); ok && i < n; i++) {
    ok = bool(new_var(ftype.param(i)));
  }
  for (size_t i = 0, n = ftype.result_n(); ok && i < n; i++) {
    ok = bool(new_var(ftype.result(i)));
  }
  // first label points to the function itself
  ok = ok && bool(new_label());
  if (ok) {
    Base::reset(name, ftype, labels_[0]);
  } else {
    Base::reset(Name{}, FuncType{}, Label{});
    holder_ = nullptr;
  }
  return *this;
}

Func::~Func() noexcept {
}

Node Func::get_compiled(ArchId archid) const noexcept {
  if (archid < ARCHID_N) {
    return compiled_[archid];
  } else {
    return Node{};
  }
}

Func &Func::set_compiled(ArchId archid, const Node &compiled) noexcept {
  if (archid < ARCHID_N) {
    compiled_[archid] = compiled;
    if (archid == NOARCH) {
      compiled_var_n_ = vars_.size();
    } else {
      // do not keep vars created by compile_<ARCH>()
      // otherwise different archs would interfere with each other
      vars_.truncate(compiled_var_n_);
    }
  }
  return *this;
}

Label Func::new_label() noexcept {
  Label l;
  const size_t i = labels_.size();
  if (i <= 0xFFFF) {
    l = Label::create(holder_, 0, i);
    if (l && !labels_.append(l)) {
      l = Label{};
    }
  }
  return l;
}

Var Func::new_var(Kind kind) noexcept {
  if (kind != Bad) {
    const size_t n = vars_.size() + Id::FIRST;
    const Id id(n); // Id is limited to 24 bits
    if (kind == Void || id.val() == n) {
      const Local local{kind, kind == Void ? Id{} : id};
      Var v = Var::create(holder_, local);
      if (v && vars_.append(v)) {
        return v;
      }
    }
  }
  return Var{};
}

} // namespace onejit

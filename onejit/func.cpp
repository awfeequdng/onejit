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

enum {
  BAD_VARID = 0,
  FIRST_VARID = 0x1000,
};

Func::Func() noexcept
    : holder_{}, param_n_{}, result_n_{}, body_var_n_{}, //
      ftype_{}, vars_{}, labels_{}, name_{}, body_{} {
}

Func &Func::reset(Code *holder, Name name, FuncType ftype) noexcept {
  holder_ = holder;
  body_var_n_ = 0;
  compiled_var_n_ = 0;
  ftype_ = ftype;
  vars_.clear();
  labels_.clear();
  name_ = name;
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
    param_n_ = ftype.param_n();
    result_n_ = ftype.result_n();
  } else {
    result_n_ = param_n_ = 0;
    holder_ = nullptr;
  }
  return *this;
}

Func::~Func() noexcept {
}

// convert Func to Label
Label Func::label() const noexcept {
  // return labels_[0], or Label{} if labels_ is empty
  return labels_[0];
}

/// \return i-th param, or Var{} if out-of-bounds
Var Func::param(uint16_t i) const noexcept {
  if (i < param_n_) {
    return vars_[i];
  }
  return Var{};
}

/// \return i-th result, or Var{} if out-of-bounds
Var Func::result(uint16_t i) const noexcept {
  if (i < result_n_) {
    return vars_[size_t(i) + param_n_];
  }
  return Var{};
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
    const size_t n = vars_.size() + FIRST_VARID;
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

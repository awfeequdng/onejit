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

#include <utility> // std::move()

namespace onejit {

enum {
  BAD_ID = 0,
  FIRST_VARID = 0x1000,
};

Func::Func() noexcept
    : holder_{}, param_n_{}, result_n_{}, body_var_n_{}, //
      ftype_{}, vars_{}, labels_{}, name_{}, body_{} {
}

Func::Func(String &&name, const FuncType &ftype, Code *holder) noexcept
    : holder_{holder}, param_n_{}, result_n_{}, body_var_n_{}, ftype_{ftype}, //
      vars_{}, labels_{}, name_(std::move(name)), body_{} {

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
    holder_ = nullptr;
  }
}

Func::~Func() noexcept {
}

// convert Func to Label
Label Func::label() const noexcept {
  return labels_.empty() ? Label{} : labels_[0];
}

/// \return i-th result, or Var{} if out-of-bounds
Var Func::param(uint16_t i) const noexcept {
  if (i < param_n_ && i < vars_.size()) {
    return vars_[i];
  }
  return Var{};
}

/// \return i-th result, or Var{} if out-of-bounds
Var Func::result(uint16_t i) const noexcept {
  if (i < result_n_ && size_t(i) + param_n_ < vars_.size()) {
    return vars_[i + param_n_];
  }
  return Var{};
}

Label Func::new_label() noexcept {
  Label l;
  const size_t i = labels_.size();
  if (i <= 0xFFFF) {
    l = Label::create(i, 0, holder_);
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

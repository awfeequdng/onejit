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
 *  Created on Jan 13, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/func.hpp"
#include "onejit/code.hpp"
#include "onejit/const.hpp"

#include <stdexcept>

namespace onejit {

enum { FIRST_SOFTREGID = 0x1000 };

Func::Func(Code *holder) : holder_(holder), regs_() {
}

Reg Func::new_reg(Kind kind) {
  const Reg r(kind, RegId(regs_.size() + FIRST_SOFTREGID));
  if (kind == Void || !regs_.append(r)) {
    return Reg();
  }
  return r;
}

Func &Func::add(CodeItem item) {
  holder_->add(item);
  return *this;
}

Func &Func::add(Reg r) {
  if (r.is_direct()) {
    return add(r.direct());
  }
  throw std::runtime_error("unimplemented: Func::add() of large Reg");
}

Func &Func::add(Const c) {
  if (c.is_direct()) {
    return add(c.direct());
  }
  throw std::runtime_error("unimplemented: Func::add() of large Const");
}

} // namespace onejit

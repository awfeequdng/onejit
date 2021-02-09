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
 * unary.cpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/node/unary.hpp>

namespace onejit {

ONEJIT_NOINLINE Node Unary::create(Func &func, Kind kind, Op1 op, Expr child) noexcept {
  return Base::create_indirect(func,                        //
                               NodeHeader{UNARY, kind, op}, //
                               {child});
}

ONEJIT_NOINLINE Node Unary::create(Func &func, Op1 op, Expr child) noexcept {
  Kind kind;
  if (op == NOT1) {
    kind = Bool;
  } else if (op <= BITCOPY) {
    // CAST and BITCOPY kind should be specified manually
    kind = child.kind();
  } else {
    kind = Bad;
  }
  return create(func, kind, op, child);
}

const Fmt &Unary::format(const Fmt &out, size_t depth) const {
  Op1 op = this->op();
  out << '(' << op;
  if (op == CAST || op == BITCOPY) {
    out << ' ' << kind();
  }
  return x().format(out, depth + 1) << ')';
}

} // namespace onejit

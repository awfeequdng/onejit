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
 * mem.cpp
 *
 *  Created on Jan 20, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/mem.hpp>

namespace onejit {
namespace ir {

Node Mem::create(Func &func, Kind kind, OpN op, Exprs args) noexcept {
  return Base::create_indirect(func,                  //
                               Header{MEM, kind, op}, //
                               Nodes{args.data(), args.size()});
}

const Fmt &Mem::format(const Fmt &fmt, Syntax syntax, size_t /*depth*/) const {
  fmt << '(' << op();
  if (syntax == Syntax::CapstoneCompat) {
    fmt << kind().bitsize();
  } else {
    fmt << '_' << kind().stringsuffix();
  }
  for (size_t i = 0, n = children(); i < n; i++) {
    Node node = child(i);
    if (node) {
      fmt << ' ' << node;
    }
  }
  return fmt << ')';
}

} // namespace ir
} // namespace onejit

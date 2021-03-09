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
 * call.cpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/call.hpp>
#include <onejit/ir/functype.hpp>
#include <onejit/ir/label.hpp>
#include <onestl/buffer.hpp>

namespace onejit {
namespace ir {

// ============================  Call  ====================================

// shortcut for child(i + 2).is<Expr>()
Expr Call::arg(uint32_t i) const noexcept {
  return child(add_uint32(2, i)).is<Expr>();
}

Node Call::create(Func &caller, const FuncType &ftype, const Label &flabel, Exprs args) noexcept {
  const size_t n = args.size();
  Code *holder = caller.code();
  while (holder && n == uint32_t(n)) {
    const Header header{TUPLE, ftype.param_n() == 0 ? Void : ftype.param(0), CALL};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(add_uint32(2, n)) && //
        holder->add(ftype, offset) && holder->add(flabel, offset) &&   //
        holder->add(args, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Node{};
}

} // namespace ir
} // namespace onejit

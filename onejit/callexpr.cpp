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
 * callexpr.cpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/callexpr.hpp>
#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/functype.hpp>
#include <onejit/label.hpp>
#include <onestl/buffer.hpp>

namespace onejit {

// ============================  Call  ====================================

std::ostream &operator<<(std::ostream &out, const Call &call) {
  out << '(' << call.op();
  // skip child(0) i.e. FuncType
  for (size_t i = 1, n = call.children(); i < n; i++) {
    out << ' ' << call.child(i);
  }
  return out << ')';
}

// ============================  Call  =====================================

// shortcut for child(0).is<FuncType>()
FuncType Call::ftype() const noexcept {
  return child(0).is<FuncType>();
}

// shortcut for child(1).is<Label>()
Label Call::label() const noexcept {
  return child(1).is<Label>();
}

// shortcut for child(i + 2).is<Expr>()
Expr Call::arg(uint32_t i) const noexcept {
  return child(sum_uint32(2, i)).is<Expr>();
}

Call Call::create(const FuncType &ftype, const Label &flabel, Exprs args, Code *holder) noexcept {
  const size_t n = args.size();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{CALL, ftype.param_n() == 0 ? Void : ftype.param(0), CALL_OP};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(sum_uint32(2, n)) && //
        holder->add(ftype, offset) && holder->add(flabel, offset) &&   //
        holder->add(args, offset)) {
      return Call{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return Call{};
}

} // namespace onejit

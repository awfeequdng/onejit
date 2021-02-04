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
 * tuple.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/functype.hpp>
#include <onejit/label.hpp>
#include <onejit/tuple.hpp>
#include <onestl/buffer.hpp>

namespace onejit {

// ============================  Tuple  ====================================

Node Tuple::create(Func &func, Kind kind, OpN op, Nodes nodes) noexcept {
  const size_t n = nodes.size();
  Code *holder = func.code();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{TUPLE, kind, op};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(n) && //
        holder->add(nodes, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Tuple{};
}

static Tuple::formatter_func formatter_vec[opn_end] = {};

// add a custom formatter_vec for operator<< on Tuple subclass
bool Tuple::register_formatter(OpN op, formatter_func func) noexcept {
  if (op < opn_end) {
    formatter_vec[op] = func;
    return true;
  }
  return false;
}

const Fmt &operator<<(const Fmt &out, const Tuple &expr) {
  OpN op = expr.op();
  if (op < opn_end) {
    if (Tuple::formatter_func func = formatter_vec[op]) {
      return func(out, expr);
    }
  }

  out << '(' << op;
  const bool is_call = op == CALL_OP;
  // if op == CALL_OP, skip child(0) i.e. FuncType
  for (size_t i = size_t(is_call), n = expr.children(); i < n; i++) {
    out << ' ' << expr.child(i);
  }
  return out << ')';
}

} // namespace onejit

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
 * unaryexpr.cpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/unaryexpr.hpp>

namespace onejit {

UnaryExpr ONEJIT_NOINLINE UnaryExpr::create(Kind kind, Op1 op, const Expr &child,
                                            Code *holder) noexcept {
  while (holder) {
    const NodeHeader header{UNARY, kind, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add(child, offset)) {
      return UnaryExpr{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return UnaryExpr{};
}

UnaryExpr UnaryExpr::create(Op1 op, const Expr &child, Code *holder) noexcept {
  Kind kind;
  if (op == XOR1 || op == CAST) {
    // CAST kind should be specified manually
    kind = child.kind();
  } else if (op == NOT1) {
    kind = Bool;
  } else {
    kind = Bad;
  }
  return create(kind, op, child, holder);
}

std::ostream &operator<<(std::ostream &out, const UnaryExpr &ue) {
  out << '(' << ue.op();
  if (ue.op() == CAST) {
    out << ' ' << ue.kind();
  }
  return out << ' ' << ue.child(0) << ')';
}

} // namespace onejit

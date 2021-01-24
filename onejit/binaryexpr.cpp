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
 * binaryexpr.cpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/binaryexpr.hpp>
#include <onejit/code.hpp>

namespace onejit {

// autodetect kind
BinaryExpr BinaryExpr::create(Op2 op, const Expr &left, const Expr &right, Code *holder) {
  while (holder) {
    Kind kind = Bad;
    if (op == BAD2) {
    } else if (op <= AND_NOT) {
      kind = left.kind();
    } else if (op <= GEQ) {
      kind = Bool; // && || comparison
    }
    const NodeHeader header{BINARY, kind, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add(left, offset) && holder->add(right, offset)) {
      return BinaryExpr{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return BinaryExpr{};
}

std::ostream &operator<<(std::ostream &out, const BinaryExpr &expr) {
  return out << '(' << expr.op() << ' ' << expr.x() << ' ' << expr.y() << ')';
}

} // namespace onejit

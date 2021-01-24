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
 * stmt3.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/stmt3.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ============================  Stmt3  ========================================

Node ONEJIT_NOINLINE Stmt3::create(Code *holder, Nodes children, OpStmt3 op) noexcept {
  while (holder && children.size() == 3) {
    const NodeHeader header{STMT_3, Void, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add(children, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Stmt3{op};
}

std::ostream &operator<<(std::ostream &out, const Stmt3 &st) {
  return out << '(' << st.op() << ' ' << st.child(0) << Chars("\n    ") << st.child(1)
             << Chars("\n    ") << st.child(2) << ')';
}

// ============================  If  =======================================

Node If::create(Code *holder, Expr cond, Node then, Node else_) noexcept {
  const Node buf[] = {cond, then, else_};
  return Stmt3::create(holder, Nodes{buf, 3}, IF);
}

} // namespace onejit

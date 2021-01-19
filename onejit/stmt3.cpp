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
 *  Created on Jan 18, 2020
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/stmt3.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ============================  Stmt3  ========================================

Stmt3 ONEJIT_NOINLINE Stmt3::create(OpStmt3 op, const Node &child0, const Node &child1,
                                    const Node &child2, Code *holder) {
  const NodeHeader header{STMT_3, Void, uint16_t(op)};
  CodeItem offset = holder->length();

  if (!holder->add(header) || !holder->add(child0, offset) || !holder->add(child1, offset) ||
      !holder->add(child2, offset)) {
    holder->truncate(offset);
    return Stmt3{op};
  }
  return Stmt3{Node{header, offset, holder}};
}

std::ostream &operator<<(std::ostream &out, const Stmt3 &st) {
  return out << '(' << st.op() << ' ' << st.child(0) << Chars("\n    ") << st.child(1)
             << Chars("\n    ") << st.child(2) << ')';
}

// ============================  IfStmt  =======================================

} // namespace onejit

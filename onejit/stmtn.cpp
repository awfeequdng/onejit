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
 * stmtn.cpp
 *
 *  Created on Jan 18, 2020
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/stmtn.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ============================  StmtN  ========================================

StmtN StmtN::create(OpStmtN op, const Nodes nodes, Code *holder) noexcept {
  while (holder) {
    const NodeHeader header{STMT_3, Void, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add(nodes.size()) && holder->add(nodes, offset)) {
      return StmtN{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return StmtN{op};
}

std::ostream &operator<<(std::ostream &out, const StmtN &st) {
  out << '(' << st.op();
  for (size_t i = 0, n = st.children(); i < n; i++) {
    out << Chars("\n    ") << st.child(i);
  }
  return out << Chars("\n)");
}

// ============================  BlockStmt  =======================================

} // namespace onejit

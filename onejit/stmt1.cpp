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
 * stmt1.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/compiler.hpp>
#include <onejit/stmt1.hpp>

namespace onejit {

// ============================  Stmt1  ========================================

Stmt1 ONEJIT_NOINLINE Stmt1::create(OpStmt1 op, const Node &child, Code *holder) noexcept {
  while (holder) {
    const NodeHeader header{STMT_1, Void, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add(child, offset)) {
      return Stmt1{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }

  return Stmt1{op};
}

Compiler &Stmt1::compile(Compiler &comp) const noexcept {
  return comp.add(*this);
}

std::ostream &operator<<(std::ostream &out, const Stmt1 &st) {
  return out << '(' << st.op() << ' ' << st.child(0) << ')';
}

// ============================  GotoStmt  ==================================

} // namespace onejit

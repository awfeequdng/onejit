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
 * stmt4.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/stmt4.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// ============================  Stmt4  ========================================

ONEJIT_NOINLINE Node Stmt4::create(Func &func, const Node &child0, const Node &child1,
                                   const Node &child2, const Node &child3, OpStmt4 op) noexcept {
  while (Code *holder = func.code()) {
    const NodeHeader header{STMT_4, Void, uint16_t(op)};
    CodeItem offset = holder->length();

    if (holder->add(header) &&                                        //
        holder->add(child0, offset) && holder->add(child1, offset) && //
        holder->add(child2, offset) && holder->add(child3, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Stmt4{op};
}

const Fmt &operator<<(const Fmt &out, const Stmt4 &st) {
  return out << '(' << st.op() << ' ' << st.child(0) << ' ' << st.child(1) //
             << ' ' << st.child(2) << "\n    " << st.child(3) << ')';
}

// ============================  For  =======================================

} // namespace onejit

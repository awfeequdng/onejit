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
 * tupleexpr.cpp
 *
 *  Created on Jan 22, 2020
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/functype.hpp>
#include <onejit/label.hpp>
#include <onejit/tupleexpr.hpp>

namespace onejit {

// ============================  TupleExpr  ====================================

std::ostream &operator<<(std::ostream &out, const TupleExpr &tuple) {
  out << '(' << tuple.op();
  // skip child(0) i.e. FuncType
  for (size_t i = 1, n = tuple.children(); i < n; i++) {
    out << ' ' << tuple.child(i);
  }
  return out << ')';
}

// ============================  CallExpr  =====================================

CallExpr CallExpr::create(const FuncType &ftype, const Label &flabel, Exprs args,
                          Code *holder) noexcept {
  const size_t n = args.size();
  while (holder && n == uint32_t(n)) {
    const NodeHeader header{TUPLE, ftype.param_n() == 0 ? Void : ftype.param(0), CALL};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint32(sum_uint32(2, n)) && //
        holder->add(ftype, offset) && holder->add(flabel, offset) &&   //
        holder->add(args, offset)) {
      return CallExpr{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return CallExpr{};
}

Compiler &CallExpr::compile(Compiler &comp) const noexcept {
  // TODO
  return comp;
}

} // namespace onejit

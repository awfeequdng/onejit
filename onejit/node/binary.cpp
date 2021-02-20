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
 * binary.cpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/node/binary.hpp>

namespace onejit {

// autodetect kind
Node Binary::create(Func &func, Op2 op, const Expr &left, const Expr &right) {
  Kind kind = Bad;
  if (op == BAD2) {
  } else if (op <= SHR) {
    kind = left.kind();
  } else if (op <= GEQ) {
    kind = Bool; // && || comparison
  }

  return Base::create_indirect(func,                         //
                               NodeHeader{BINARY, kind, op}, //
                               {left, right});
}

const Fmt &Binary::format(const Fmt &out, Syntax syntax, size_t depth) const {
  out << '(' << op() << ' ';
  x().format(out, syntax, depth + 1) << ' ';
  return y().format(out, syntax, depth + 1) << ')';
}

} // namespace onejit

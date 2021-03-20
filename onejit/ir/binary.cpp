/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * binary.cpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/binary.hpp>

namespace onejit {
namespace ir {

// autodetect kind
Node Binary::create(Func &func, Op2 op, const Expr &left, const Expr &right) {
  Kind kind = Bad;
  if (op == BAD2) {
  } else if (op <= SHR) {
    kind = left.kind();
  } else if (op <= GEQ) {
    kind = Bool; // && || comparison
  }

  return Base::create_indirect(func,                     //
                               Header{BINARY, kind, op}, //
                               {left, right});
}

const Fmt &Binary::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  fmt << '(' << op() << ' ';
  x().format(fmt, syntax, depth + 1) << ' ';
  return y().format(fmt, syntax, depth + 1) << ')';
}

} // namespace ir
} // namespace onejit

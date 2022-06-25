/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * mem.cpp
 *
 *  Created on Jan 20, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/mem.hpp>

namespace onejit {
namespace ir {

Node Mem::create(Func &func, Kind kind, OpN op, Exprs args) noexcept {
  return Base::create_indirect(func,                  //
                               Header{MEM, kind, op}, //
                               Nodes{args.data(), args.size()});
}

const Fmt &Mem::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  fmt << '(' << op();
  if (syntax == Syntax::CapstoneCompat) {
    fmt << kind().bitsize();
  } else {
    fmt << '_' << kind().stringsuffix();
  }
  for (size_t i = 0, n = children(); i < n; i++) {
    Node node = child(i);
    if (node) {
      fmt << ' ';
      node.format(fmt, syntax, depth + 1);
    }
  }
  return fmt << ')';
}

} // namespace ir
} // namespace onejit

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
 * optimizer_tuple.cpp
 *
 *  Created on Feb 14, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/eval.hpp>
#include <onejit/node/const.hpp>
#include <onejit/node/tuple.hpp>
#include <onejit/optimizer.hpp>

#include <algorithm>

namespace onejit {

Expr Optimizer::partial_eval_tuple(Tuple expr, Span<Node> children) noexcept {
  OpN op = expr.op();
  bool have_value = false;
  if (is_associative(op) && is_commutative(op) && (flags_ & FastMath || !expr.kind().is_float())) {
    std::sort(children.begin(), children.end(),
              [](const Node &lhs, const Node &rhs) { return lhs.type() < rhs.type(); });
    size_t n = children.size();
    Value v;
    for (; n != 0; --n) {
      Const c = children[n - 1].is<Const>();
      if (!c) {
        break;
      } else if (have_value) {
        v = eval_tuple(op, {c.val(), v});
      } else {
        have_value = true;
        v = c.val();
      }
    }
    if (have_value) {
      Expr e = Const{*func_, v};
      children[n] = e;
      children.truncate(n + 1);
      return n == 1 ? e : Tuple{*func_, expr.kind(), op, children};
    }
  }
  return Expr{};
}

} // namespace onejit

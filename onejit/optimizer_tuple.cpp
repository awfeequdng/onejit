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
#include <onejit/node/var.hpp>
#include <onejit/optimizer.hpp>

#include <algorithm>

namespace onejit {

template <class T> //
static bool all_are(Nodes nodes) noexcept {
  for (Node node : nodes) {
    if (!node.is<T>()) {
      return false;
    }
  }
  return true;
}

Expr Optimizer::partial_eval_tuple(Tuple expr, Span<Node> children) noexcept {
  OpN op = expr.op();
  Kind kind = expr.kind();
  if (is_associative(op) && is_commutative(op) && (flags_ & FastMath || !kind.is_float())) {
    Value identity = Value::identity(op, kind);
    size_t n = children.size();
    if (n == 0) {
      return Const{*func_, identity};
    } else if (n > 1) {
      // also put constants as last
      std::sort(children.begin(), children.end(),
                [](const Node &lhs, const Node &rhs) { return lhs.type() < rhs.type(); });
    }
    Value v = identity;
    for (; n != 0; --n) {
      if (Const c = children[n - 1].is<Const>()) {
        v = eval_tuple(op, {c.val(), v});
      } else {
        break;
      }
    }
    if (n < children.size() && v != identity) {
      Const c{*func_, v};
      if (v == Value::absorbing(op, kind) && all_are<Var>(children.view(0, n))) {
        return c;
      }
      children[n++] = c;
    }
    switch (n) {
    case 0:
      return Const{*func_, v};
    case 1:
      return children[0].is<Expr>();
    default:
      return Tuple{*func_, kind, op, children.view(0, n)};
    }
  }
  return Expr{};
}

} // namespace onejit

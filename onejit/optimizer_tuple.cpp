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

Expr Optimizer::optimize(Tuple expr, bool optimize_children) noexcept {
  Expr ret;
  if (expr && (flags_ & OptSimplifyExpr)) {
    size_t orig_n = nodes_.size();
    if (flatten_children_tobuf(expr, optimize_children)) {
      NodeRange children{&nodes_, orig_n, nodes_.size() - orig_n};
      ret = partial_eval_tuple(expr, children);
    }
    nodes_.truncate(orig_n);
  }
  return ret ? ret : expr;
}

bool Optimizer::flatten_children_tobuf(Node node, bool optimize_children) noexcept {
  bool ok = true;
  for (size_t i = 0, n = node.children(); ok && i < n; i++) {
    Node child = node.child(i);
    // compare type, kind, op
    if (child.header() == node.header()) {
      ok = flatten_children_tobuf(child, optimize_children);
    } else {
      if (optimize_children) {
        child = optimize(child);
      }
      if (child.header() == node.header()) {
        ok = flatten_children_tobuf(child, optimize_children);
      } else {
        ok = bool(nodes_.append(child));
      }
    }
  }
  return ok;
}

Expr Optimizer::partial_eval_tuple(Tuple expr, NodeRange &children) noexcept {
  OpN op = expr.op();
  Kind kind = expr.kind();
  if (is_associative(op) && (flags_ & OptFastMath || !kind.is_float())) {
    Value identity = Value::identity(kind, op);
    size_t n = children.size();
    if (n == 0) {
      return Const{*func_, identity};
    } else if (n > 1 && is_commutative(op)) {
      // also put constants as last
      std::sort(children.begin(), children.end(),      //
                [](const Node &lhs, const Node &rhs) { //
                  return lhs.deep_compare(rhs) < 0;
                });
    }
    Value v = identity;
    if (flags_ & OptFoldConstant) {
      for (; n != 0; --n) {
        if (Const c = children[n - 1].is<Const>()) {
          v = eval_tuple(kind, op, {c.val(), v});
        } else {
          break;
        }
      }
      if (n < children.size() && v != identity) {
        Const c{*func_, v};
        if (v == Value::absorbing(kind, op) && all_are<Var>(children.to_nodes().view(0, n))) {
          return c;
        }
        children[n++] = c;
      }
      children.truncate(n);
    }
    switch (n) {
    case 0:
      return Const{*func_, v};
    case 1:
      return children[0].is<Expr>();
    default:
      break;
    }
  }
  if (same_children(expr, children)) {
    return expr;
  }
  return Tuple{*func_, kind, op, children.to_nodes()};
}

} // namespace onejit

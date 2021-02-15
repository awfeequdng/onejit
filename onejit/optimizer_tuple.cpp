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
#include <onejit/optimizer_result.hpp>

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

static bool same_children(Node node, Nodes children) noexcept {
  const size_t n = node.children();
  if (n != children.size()) {
    return false;
  }
  for (size_t i = 0; i < n; i++) {
    if (node.child(i) != children[i]) {
      return false;
    }
  }
  return true;
}

Expr Optimizer::optimize(Tuple expr, Result &in_out) noexcept {
  Expr ret;
  Result result = IsNone;
  if (expr && (flags_ & ExprSimplification)) {
    size_t orig_n = nodes_.size();
    result = IsAll;
    if (optimize_flatten_children_tobuf(expr, result)) {
      // Caveats:
      //
      // 1. do not use nodes_.span() here, because it may throw.
      //
      // 2. optimize_flatten_children_tobuf() above appends to nodes_
      //    and thus may reallocate its data()
      //    => we must retrieve nodes_.data() *after* such call.
      if (nodes_.size() >= orig_n) {
        Span<Node> children{nodes_.data() + orig_n, nodes_.size() - orig_n};
        ret = partial_eval_tuple(expr, children);
        nodes_.truncate(orig_n);
      }
    } else {
      result = IsNone;
    }
  }
  in_out &= result;
  return ret ? ret : expr;
}

Expr Optimizer::flatten_tuple(Tuple expr) noexcept {
  Expr expr2;
  if (expr && (flags_ & ExprSimplification)) {
    size_t orig_n = nodes_.size();
    if (flatten_children_tobuf(expr)) {
      const size_t n = nodes_.size() - orig_n;
      Span<Node> children{nodes_.data() + orig_n, n};
      expr2 = partial_eval_tuple(expr, children);
    }
    nodes_.truncate(orig_n);
  }
  return expr2 ? expr2 : expr;
}

bool Optimizer::flatten_children_tobuf(Node node) noexcept {
  bool ok = true;
  for (size_t i = 0, n = node.children(); ok && i < n; i++) {
    Node child = node.child(i);
    // compare type, kind, op
    if (child.header() == node.header()) {
      ok = flatten_children_tobuf(child);
    } else {
      ok = bool(nodes_.append(child));
    }
  }
  return ok;
}

bool Optimizer::optimize_flatten_children_tobuf(Node node, Result &result) noexcept {
  bool ok = true;
  for (size_t i = 0, n = node.children(); ok && i < n; i++) {
    Node child = node.child(i);
    // compare type, kind, op
    if (child.header() == node.header()) {
      result &= ~IsSame;
      ok = optimize_flatten_children_tobuf(child, result);
    } else {
      child = optimize(child, result);
      if (child.header() == node.header()) {
        ok = flatten_children_tobuf(child);
      } else {
        ok = bool(nodes_.append(child));
      }
    }
  }
  return ok;
}

Expr Optimizer::partial_eval_tuple(Tuple expr, Span<Node> children) noexcept {
  OpN op = expr.op();
  Kind kind = expr.kind();
  if (is_associative(op) && (flags_ & FastMath || !kind.is_float())) {
    Value identity = Value::identity(kind, op);
    size_t n = children.size();
    if (n == 0) {
      return Const{*func_, identity};
    } else if (n > 1 && is_commutative(op)) {
      // also put constants as last
      std::sort(children.begin(), children.end(),
                [](const Node &lhs, const Node &rhs) { return lhs.type() < rhs.type(); });
    }
    Value v = identity;
    if (flags_ & ConstantFolding) {
      for (; n != 0; --n) {
        if (Const c = children[n - 1].is<Const>()) {
          v = eval_tuple(kind, op, {c.val(), v});
        } else {
          break;
        }
      }
      if (n < children.size() && v != identity) {
        Const c{*func_, v};
        if (v == Value::absorbing(kind, op) && all_are<Var>(children.view(0, n))) {
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
  return Tuple{*func_, kind, op, children};
}

} // namespace onejit

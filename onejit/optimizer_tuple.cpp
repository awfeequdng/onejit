/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * optimizer_tuple.cpp
 *
 *  Created on Feb 14, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/eval.hpp>
#include <onejit/ir/const.hpp>
#include <onejit/ir/tuple.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/optimizer.hpp>
#include <onestl/range.hpp>

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
      Range<Node> children{&nodes_, orig_n, nodes_.size()};
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

Expr Optimizer::partial_eval_tuple(Tuple expr, Range<Node> &noderange) noexcept {
  Span<Node> children = noderange.span();
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
        if (v == Value::absorbing(kind, op) && all_are<Var>(children.view(0, n))) {
          return c;
        }
        children.set(n++, c);
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

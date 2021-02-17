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
 * optimizer.cpp
 *
 *  Created on Feb 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/eval.hpp>
#include <onejit/func.hpp>
#include <onejit/node/binary.hpp>
#include <onejit/node/const.hpp>
#include <onejit/node/stmt2.hpp>
#include <onejit/node/tuple.hpp>
#include <onejit/node/unary.hpp>
#include <onejit/optimizer.hpp>
#include <onestl/vector.hpp>

namespace onejit {

Optimizer::Optimizer() noexcept : func_{nullptr}, nodes_{}, check_{CheckNone}, flags_{OptNone} {
}

Optimizer::~Optimizer() noexcept {
}

bool Optimizer::same_children(Node node, Nodes children) noexcept {
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

Node Optimizer::optimize(Func &func, Node node, Opt flags) noexcept {
  if (func && node && flags != OptNone) {
    func_ = &func;
    nodes_.clear();
    flags_ = flags;
    node = optimize(node);
  }
  return node;
}

Node Optimizer::optimize(Node node) noexcept {
  if (!*func_) {
    // out of memory
    return node;
  }
  uint32_t n = node.children();
  Type t = node.type();
  if (t >= LABEL && n == 0) {
    return node;
  } else if (t == TUPLE) {
    return optimize(node.is<Tuple>(), true);
  }

  Node new_node;
  // use a NodeRange on nodes_ because a span or view would be invalidated
  // by try_optimize() calling back optimize() which may resize nodes_
  // and change its data()
  NodeRange children = {nullptr, 0, 0};
  size_t orig_n = nodes_.size();

  if (!optimize_children(node, children)) {
    nodes_.truncate(orig_n);
    return node;
  } else if (t == UNARY) {
    new_node = try_optimize(node.is<Unary>(), children);
  } else if (t == BINARY) {
    new_node = try_optimize(node.is<Binary>(), children);
  } else if (t == STMT_2 && is_assign(OpStmt2(node.op()))) {
    new_node = try_optimize(node.is<Assign>(), children);
  }
  if (!new_node && !same_children(node, children.to_view())) {
    new_node = Node::create_indirect(*func_, node.header(), children.to_view());
  }
  nodes_.truncate(orig_n);

  return new_node ? new_node : node;
}

bool Optimizer::optimize_children(Node node, NodeRange &children) noexcept {
  size_t n = node.children();
  size_t orig_n = nodes_.size();

  if (!nodes_.resize(n + orig_n)) {
    return false;
  }
  for (size_t i = 0; i < n; i++) {
    // optimize() may resize nodes_ and change its data()
    // => we must split assignment in two statements
    Node child = optimize(node.child(i));
    nodes_[i + orig_n] = child;
  }
  children = NodeRange{&nodes_, orig_n, n};
  return true;
}

Node Optimizer::try_optimize(Unary expr, const NodeRange &children) noexcept {
  Expr x;
  if (expr && children.size() == 1 && (x = children[0].is<Expr>())) {
    Kind kind = expr.kind();
    Op1 op = expr.op();

    if ((flags_ & OptFoldConstant) && x.type() == CONST) {
      Value v0 = x.is<Const>().val();
      if (v0.is_valid()) {
        Value ve = eval_unary(kind, op, v0);
        if (ve.is_valid()) {
          return Const{*func_, ve};
        }
      }
    }
    if (flags_ & OptSimplifyExpr) {
      return simplify_unary(kind, op, x);
    }
  }
  return Node{};
}

Expr Optimizer::simplify_unary(Kind kind, Op1 op, Expr x) noexcept {
  if (Unary u = x.is<Unary>()) {
    Op1 xop = u.op();
    if (Expr xx = u.x()) {
      if (op == XOR1 && xop == XOR1) {
        // simplify ~~xx to xx
        return xx;
      } else if (op == NEG1 && xop == NEG1) {
        // simplify --xx to xx
        return xx;
      } else if (op == NEG1 && xop == XOR1) {
        // simplify -~xx to xx+1
        return Tuple{*func_, ADD, xx, One(*func_, xx.kind())};
      } else if (op == XOR1 && xop == NEG1) {
        // simplify ~-xx to xx-1
        return Binary{*func_, SUB, xx, One(*func_, xx.kind())};
      }
    }
  } else if (Binary b = x.is<Binary>()) {
    Op2 bop = b.op();
    if (op == NOT1 && is_comparison(bop)) {
      // simplify !(a compare b) to (a inverted_compare b)
      return Binary{*func_, not_comparison(bop), b.x(), b.y()};
    }
  }
  if ((op == CAST || op == BITCOPY) && kind == x.kind()) {
    // CAST or BITCOPY from a kind to itself
    return x;
  }
  return Expr{};
}

Node Optimizer::try_optimize(Assign st, const NodeRange &children) noexcept {
  // TODO
  (void)st;
  (void)children;
  return Node{};
}

} // namespace onejit

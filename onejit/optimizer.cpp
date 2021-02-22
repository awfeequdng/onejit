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
#include <onejit/node/stmt1.hpp>
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

  size_t orig_n = nodes_.size();
  Node new_node;
  // use a Range<Node> on nodes_ because a span or view would be invalidated
  // by try_optimize() calling back optimize() which may resize nodes_
  // and change its data()
  Range<Node> children = optimize_children(node);

  if (!children) {
    nodes_.truncate(orig_n);
    return node;
  } else if (Unary unary = node.is<Unary>()) {
    new_node = try_optimize(unary, children);
  } else if (Binary binary = node.is<Binary>()) {
    new_node = try_optimize(binary, children);
  } else if (Assign assign = node.is<Assign>()) {
    new_node = try_optimize(assign, children);
  }
  if (!new_node && !same_children(node, children.view())) {
    new_node = Node::create_indirect(*func_, node.header(), children.view());
  }
  nodes_.truncate(orig_n);

  return new_node ? new_node : node;
}

Range<Node> Optimizer::optimize_children(Node node) noexcept {
  size_t n = node.children();
  size_t orig_n = nodes_.size();

  if (!nodes_.resize(n + orig_n)) {
    return Range<Node>{};
  }
  for (size_t i = 0; i < n; i++) {
    // optimize() may resize nodes_ and change its data()
    // => do not take references to nodes_.data() before calling optimize(),
    // as STL operator[] would do
    nodes_.set(i + orig_n, optimize(node.child(i)));
  }
  return Range<Node>{&nodes_, orig_n, n};
}

Node Optimizer::try_optimize(Unary expr, const Range<Node> &children) noexcept {
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

Node Optimizer::try_optimize(Assign st, const Range<Node> &children) noexcept {
  return try_optimize(st.op(), children[0].is<Expr>(), children[1].is<Expr>());
}

Node Optimizer::try_optimize(OpStmt2 assign_op, Expr dst, Expr src) noexcept {
  if (src && dst) {
    Kind kind = dst.kind();
    if (Const c = src.is<Const>()) {
      if (Node ret = simplify_assign(assign_op, dst, c.val())) {
        return ret;
      }
    } else if (dst.deep_equal(src, allow_mask_pure())) {
      // optimize (op= expr expr)
      switch (assign_op) {
      case ADD_ASSIGN:
        return Assign{*func_, MUL_ASSIGN, dst, Two(*func_, kind)};
      case SUB_ASSIGN:
      case REM_ASSIGN:
      case XOR_ASSIGN:
        return Assign{*func_, ASSIGN, dst, Zero(kind)};
      case MUL_ASSIGN:
        break; // square
      case QUO_ASSIGN:
        return Assign{*func_, ASSIGN, dst, One(*func_, kind)};
      case AND_ASSIGN:
      case OR_ASSIGN:
      case ASSIGN:
        return VoidExpr;
      default:
        break;
      }
    } else if (assign_op == ASSIGN) {
      if (Binary bsrc = src.is<Binary>()) {
        if (OpStmt2 op = to_assign_op(bsrc.op())) {
          if (dst.deep_equal(bsrc.x(), allow_mask_pure())) {
            // optimize (= dst (op dst y)) to (op= dst y)
            return make_assign(op, dst, bsrc.y());
          }
        }
      } else if (Tuple tsrc = src.is<Tuple>()) {
        if (OpStmt2 op = to_assign_op(tsrc.op())) {
          if (tsrc.children() == 2 //
              && dst.deep_equal(tsrc.arg(0), allow_mask_pure())) {
            // optimize (= dst (op dst y)) to (op= dst y)
            return make_assign(op, dst, tsrc.arg(1));
          }
        }
      }
    }
  }
  return Node{};
}

Node Optimizer::make_assign(OpStmt2 assign_op, Expr dst, Expr src) noexcept {
  if (Node ret = try_optimize(assign_op, dst, src)) {
    return ret;
  }
  return Assign{*func_, assign_op, dst, src};
}

// try to simplify (op= dst const)
Node Optimizer::simplify_assign(OpStmt2 assign_op, Expr dst, Value val) noexcept {
  Kind kind = dst.kind();
  if (OpN op = to_opn(assign_op)) {
    Value absorbing = Value::absorbing(kind, op);
    if (val == absorbing) {
      // optimize (op= expr absorbing) to (= expr absorbing)
      return Assign{*func_, ASSIGN, dst, Const{*func_, absorbing}};
    } else if (val == Value::identity(kind, op) && dst.deep_pure(allow_mask_pure())) {
      // optimize (op= expr identity) to nothing
      return VoidExpr;
    } else if (op == ADD) {
      if (val == Value::one(kind)) {
        // optimize (+= expr 1) to (inc expr)
        return Inc{*func_, dst};
      } else if (val == Value::minus_one(kind)) {
        // optimize (+= expr -1) to (dec expr)
        return Dec{*func_, dst};
      }
    }
    return Node{};
  }
  if (Op2 op = to_op2(assign_op)) {
    if (val == Value::identity(kind, op) && dst.deep_pure(allow_mask_pure())) {
      // optimize (op= expr identity) to nothing
      return VoidExpr;
    } else if (op == SUB) {
      if (val == Value::one(kind)) {
        // optimize (-= expr 1) to (dec expr)
        return Dec{*func_, dst};
      } else if (val == Value::minus_one(kind)) {
        // optimize (-= expr -1) to (inc expr)
        return Inc{*func_, dst};
      }
    }
  }
  return Node{};
}

} // namespace onejit

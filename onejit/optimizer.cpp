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
#include <onejit/node/unary.hpp>
#include <onejit/optimizer.hpp>
#include <onestl/vector.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////

enum Optimizer::Result : uint8_t {
  IsNone = 0,
  IsSame = 1 << 0,
  IsPure = 1 << 1,
  IsConst = 1 << 2,
  IsAll = 0x7,
};

constexpr Optimizer::Result operator~(Optimizer::Result a) noexcept {
  return Optimizer::Result(~uint8_t(a));
}

constexpr Optimizer::Result operator&(Optimizer::Result a, Optimizer::Result b) noexcept {
  return Optimizer::Result(uint8_t(a) & uint8_t(b));
}

constexpr Optimizer::Result operator|(Optimizer::Result a, Optimizer::Result b) noexcept {
  return Optimizer::Result(uint8_t(a) | uint8_t(b));
}

Optimizer::Result &operator&=(Optimizer::Result &a, Optimizer::Result b) noexcept {
  return a = a & b;
}

Optimizer::Result &operator|=(Optimizer::Result &a, Optimizer::Result b) noexcept {
  return a = a | b;
}

////////////////////////////////////////////////////////////////////////////////

Optimizer::Optimizer() noexcept : func_{nullptr}, nodes_{}, flags_{None} {
}

Optimizer::~Optimizer() noexcept {
}

Node Optimizer::optimize(Func &func, Node node, Flag flags) noexcept {
  if (func && node && flags != None) {
    func_ = &func;
    nodes_.clear();
    flags_ = flags;
    Result result = IsAll;
    node = optimize(node, result);
  }
  return node;
}

Node Optimizer::optimize(Node node, Result &in_out) noexcept {
  if (!*func_) {
    // out of memory
    in_out &= IsSame;
    return node;
  }
  uint32_t n = node.children();
  Type t = node.type();
  if (optimize_leaf(t, n, in_out)) {
    return node;
  }
  Node new_node;
  size_t orig_n = nodes_.size();
  if (!nodes_.resize(n + orig_n)) {
    in_out &= IsSame;
    return node;
  }
  Result result = IsAll;
  for (uint32_t i = 0; i < n; i++) {
    nodes_[i + orig_n] = optimize(node.child(i), result);
  }
  // Caveats:
  //
  // 1. do not use nodes_.view() here, because it may throw.
  //
  // 2. the loop immediately above recursively calls optimize()
  //    which may resize nodes_ and thus change its data()
  //    => we must retrieve nodes_.data() *after* such loop.
  Nodes children{nodes_.data() + orig_n, n};
  switch (node.type()) {
  case UNARY:
    new_node = optimize(node.is<Unary>(), children, result);
    break;
  case BINARY:
    new_node = optimize(node.is<Binary>(), children, result);
    break;
  default:
    break;
  }

  if (!new_node && !(result & IsSame)) {
    new_node = Node::create_indirect(*func_, node.header(), children);
  }
  nodes_.truncate(orig_n);

  return finish(node, new_node, result, in_out);
}

Node Optimizer::finish(Node node, Node new_node, Result result, Result &in_out) noexcept {
  if (new_node && new_node != node) {
    result &= ~IsSame;
  } else {
    new_node = node;
    result |= IsSame;
  }
  if (new_node.type() == CONST) {
    result |= IsConst | IsPure;
  } else {
    result &= ~IsConst;
  }
  in_out &= result;
  return new_node;
}

bool Optimizer::optimize_leaf(Type t, size_t n_children, Result &in_out) noexcept {
  if (n_children == 0 || t >= LABEL) {
    if (t != CONST) {
      in_out &= ~IsConst;
    }
    if (t == LABEL) {
      // labels are jump destinations, cannot optimize them away
      in_out &= ~IsPure;
    }
    return true;
  }
  return false;
}

Node Optimizer::optimize(Unary expr, Nodes children, Result result) noexcept {
  Expr x;
  if (expr && children.size() == 1 && (x = children[0].is<Expr>())) {
    Kind kind = expr.kind();
    Op1 op = expr.op();

    if ((flags_ & ConstantFolding) && (result & IsConst)) {
      Value v0, ve;
      if ((v0 = x.is<Const>().val()).is_valid()) {
        if ((ve = eval_unary(kind, op, v0)).is_valid()) {
          return Const{*func_, ve};
        }
      }
    }
    if (flags_ & ExprSimplification) {
      return simplify_unary(kind, op, x);
    }
  }
  return Node{};
}

Node Optimizer::optimize(Binary expr, Nodes children, Result result) noexcept {
  Expr x, y;
  if (expr && children.size() == 2 //
      && (x = children[0].is<Expr>()) && (y = children[1].is<Expr>())) {

    Op2 op = expr.op();
    if ((flags_ & ConstantFolding) && (result & IsConst)) {
      Value v0, v1, ve;
      if ((v0 = x.is<Const>().val()).is_valid()) {
        if ((v1 = y.is<Const>().val()).is_valid()) {
          if ((ve = eval_binary(op, v0, v1)).is_valid()) {
            return Const{*func_, ve};
          }
        }
      }
    }
    if (flags_ & ExprSimplification) {
      return simplify_binary(op, x, y);
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
        return Binary{*func_, ADD2, xx, One(*func_, xx.kind())};
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

} // namespace onejit

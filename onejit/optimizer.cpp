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
#include <onejit/node/node.hpp>
#include <onejit/node/unary.hpp>
#include <onejit/optimizer.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////

enum Optimizer::Result : uint8_t {
  IsNone = 0,
  IsSame = 1 << 0,
  IsPure = 1 << 1,
  IsConst = 1 << 2,
  IsAll = 0xff,
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
  if (n == 0 || t >= LABEL) {
    if (t != CONST) {
      in_out &= ~IsConst;
    }
    if (t == LABEL) {
      // labels are jump destinations, cannot optimize them away
      in_out &= ~IsPure;
    }
    return node;
  }
  nodes_.clear();
  if (!nodes_.resize(n)) {
    in_out &= IsSame;
    return node;
  }
  Result result = IsAll;
  for (uint32_t i = 0; i < n; i++) {
    nodes_[i] = optimize(node.child(i), result);
  }
  Node new_node;
  switch (node.type()) {
  case UNARY:
    new_node = optimize(node.is<Unary>(), result);
    break;
  case BINARY:
    new_node = optimize(node.is<Binary>(), result);
    break;
  default:
    break;
  }
  return finish(node, new_node, result, in_out);
}

Node Optimizer::finish(Node orig_node, Node new_node, Result result, Result &in_out) noexcept {
  if (!new_node && !(result & IsSame)) {
    new_node = Node::create_indirect(*func_, orig_node.header(), nodes_);
  }

  if (new_node && new_node != orig_node) {
    result &= ~IsSame;
  } else {
    new_node = orig_node;
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

Node Optimizer::optimize(Unary expr, Result result) noexcept {
  Node new_node;
  if (expr && nodes_.size() == 1) {
    if ((flags_ & ConstantFolding) && (result & IsConst)) {
      Value v0, ve;
      if ((v0 = nodes_[0].is<Const>().imm()).is_valid()) {
        if ((ve = eval_unary(expr.kind(), expr.op(), v0)).is_valid()) {
          new_node = Const{*func_, ve};
        }
      }
    }
    if (!new_node && (flags_ & ExprSimplification)) {
      new_node = simplify(expr);
    }
  }
  return new_node;
}

Node Optimizer::optimize(Binary expr, Result result) noexcept {
  Node new_node;
  if (expr && nodes_.size() == 2) {
    if ((flags_ & ConstantFolding) && (result & IsConst)) {
      Value v0, v1, ve;
      if ((v0 = nodes_[0].is<Const>().imm()).is_valid()) {
        if ((v1 = nodes_[1].is<Const>().imm()).is_valid()) {
          if ((ve = eval_binary(expr.op(), v0, v1)).is_valid()) {
            new_node = Const{*func_, ve};
          }
        }
      }
    }
    if (!new_node && (flags_ & ExprSimplification)) {
      new_node = simplify(expr);
    }
  }
  return new_node;
}

Node Optimizer::simplify(Unary expr) noexcept {
  Node new_node;
  Op1 op = expr.op();
  if (Unary x = nodes_[0].is<Unary>()) {
    if (Expr y = x.x()) {
      Op1 xop = x.op();
      if (op == XOR1 && xop == XOR1) {
        // simplify ~~y to y
        new_node = y;
      } else if (op == XOR1 && xop == NEG1) {
        // simplify ~-y to y-1
        new_node = Binary{*func_, SUB, y, One(y.kind())};
      } else if (op == NEG1 && xop == NEG1) {
        // simplify -~y to y+1
        new_node = Binary{*func_, ADD, y, One(y.kind())};
      } else if (op == NEG1 && xop == NEG1) {
        // simplify --y to y
        new_node = y;
      }
    }
  }
  if (!new_node) {
    if (Expr x = nodes_[0].is<Expr>()) {
      if ((op == CAST || op == BITCOPY) && expr.kind() == x.kind()) {
        // CAST or BITCOPY from a kind to itself
        new_node = x;
      }
    }
  }
  return new_node;
}

Node Optimizer::simplify(Binary expr) noexcept {
  Node new_node;
  Expr x = expr.x(), y = expr.y();
  Op2 op = expr.op();
  if (is_commutative(op) && x.kind() > y.kind()) {
    new_node = Binary{*func_, op, y, x};
  }
  return new_node;
}

} // namespace onejit

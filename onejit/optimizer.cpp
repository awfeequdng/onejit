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
#include <onestl/vector.hpp>

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

Optimizer::Optimizer() noexcept : func_{nullptr}, flags_{None} {
}

Optimizer::~Optimizer() noexcept {
}

Node Optimizer::optimize(Func &func, Node node, Flag flags) noexcept {
  if (func && node && flags != None) {
    func_ = &func;
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
  Vector<Node> children;
  if (!children.resize(n)) {
    in_out &= IsSame;
    return node;
  }
  Result result = IsAll;
  for (uint32_t i = 0; i < n; i++) {
    children[i] = optimize(node.child(i), result);
  }
  Node new_node;
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
  return finish(node, new_node, Nodes{children}, result, in_out);
}

Node Optimizer::finish(const Node &orig_node, Node new_node, const Nodes &optimized_children,
                       Result result, Result &in_out) noexcept {
  if (!new_node && !(result & IsSame)) {
    new_node = Node::create_indirect(*func_, orig_node.header(), optimized_children);
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

Node Optimizer::optimize(Unary expr, Nodes children, Result result) noexcept {
  Node ret;
  if (expr && children.size() == 1) {
    if ((flags_ & ConstantFolding) && (result & IsConst)) {
      Value v0, ve;
      if ((v0 = children[0].is<Const>().imm()).is_valid()) {
        if ((ve = eval_unary(expr.kind(), expr.op(), v0)).is_valid()) {
          ret = Const{*func_, ve};
        }
      }
    }
    if (!ret && (flags_ & ExprSimplification)) {
      ret = simplify(expr, children);
    }
  }
  return ret;
}

Node Optimizer::optimize(Binary expr, Nodes children, Result result) noexcept {
  Node ret;
  if (expr && children.size() == 2) {
    if ((flags_ & ConstantFolding) && (result & IsConst)) {
      Value v0, v1, ve;
      if ((v0 = children[0].is<Const>().imm()).is_valid()) {
        if ((v1 = children[1].is<Const>().imm()).is_valid()) {
          if ((ve = eval_binary(expr.op(), v0, v1)).is_valid()) {
            ret = Const{*func_, ve};
          }
        }
      }
    }
    if (!ret && (flags_ & ExprSimplification)) {
      ret = simplify(expr, children);
    }
  }
  return ret;
}

Node Optimizer::simplify(Unary expr, Nodes children) noexcept {
  Node ret, arg = children[0];
  Op1 op = expr.op();
  if (Unary u = arg.is<Unary>()) {
    if (Expr y = u.x()) {
      Op1 xop = u.op();
      if (op == XOR1 && xop == XOR1) {
        // simplify ~~y to y
        ret = y;
      } else if (op == XOR1 && xop == NEG1) {
        // simplify ~-y to y-1
        ret = Binary{*func_, SUB, y, One(*func_, y.kind())};
      } else if (op == NEG1 && xop == XOR1) {
        // simplify -~y to y+1
        ret = Binary{*func_, ADD, y, One(*func_, y.kind())};
      } else if (op == NEG1 && xop == NEG1) {
        // simplify --y to y
        ret = y;
      }
    }
  } else if (Binary b = arg.is<Binary>()) {
    Op2 xop = b.op();
    if (op == NOT1 && is_comparison(xop)) {
      // simplify !(a compare b) to (a inverted_compare b)
      ret = Binary{*func_, not_comparison(xop), b.x(), b.y()};
    }
  }
  if (!ret) {
    if (Expr e = arg.is<Expr>()) {
      if ((op == CAST || op == BITCOPY) && expr.kind() == e.kind()) {
        // CAST or BITCOPY from a kind to itself
        ret = e;
      }
    }
  }
  return ret;
}

Node Optimizer::simplify(Binary expr, Nodes children) noexcept {
  Node ret;
  Expr x = children[0].is<Expr>(), y = children[1].is<Expr>();
  if (x && y) {
    Op2 op = expr.op();
    if (x.type() > y.type()) {
      if (is_commutative(op)) {
        // put constants to the right
        ret = expr = Binary{*func_, op, y, x};
      } else if (is_comparison(op)) {
        // put constants to the right
        ret = expr = Binary{*func_, swap_comparison(op), y, x};
      }
    }
  }
  return ret;
}

} // namespace onejit

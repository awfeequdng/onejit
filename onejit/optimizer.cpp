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

#include <onejit/const.hpp>
#include <onejit/eval.hpp>
#include <onejit/func.hpp>
#include <onejit/node/binary.hpp>
#include <onejit/node/node.hpp>
#include <onejit/node/unary.hpp>
#include <onejit/optimizer.hpp>

namespace onejit {

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
  uint32_t n = node.children();
  if (n == 0 || node.type() >= LABEL) {
    in_out = in_out & ((node.type() == CONST ? IsConst : IsNone) | IsSame | IsPure);
    if (node.type() < LABEL) {
      in_out = in_out & ~IsPure;
    }
    return node;
  }
  nodes_.clear();
  if (!nodes_.resize(n)) {
    in_out = in_out & IsSame;
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
    result = result & ~IsSame;
  } else {
    new_node = orig_node;
    result = result | IsSame;
  }
  if (new_node.type() == CONST) {
    result = result | IsConst;
  } else {
    result = result & ~IsConst;
  }
  in_out = in_out & result;
  return new_node;
}

Node Optimizer::optimize(Unary expr, Result result) noexcept {
  Node new_node = expr;
  if (expr && (result & IsConst) && (flags_ & ConstantFolding) && nodes_.size() == 1) {
    Value v0, ve;
    if ((v0 = nodes_[0].is<Const>().imm()).is_valid()) {
      if ((ve = eval_unary(expr.kind(), expr.op(), v0)).is_valid()) {
        new_node = Const{*func_, ve};
      }
    }
  }
  return new_node;
}

Node Optimizer::optimize(Binary expr, Result result) noexcept {
  Node new_node = expr;
  if (expr && (result & IsConst) && (flags_ & ConstantFolding) && nodes_.size() == 2) {
    Value v0, v1, ve;
    if ((v0 = nodes_[0].is<Const>().imm()).is_valid()) {
      if ((v0 = nodes_[1].is<Const>().imm()).is_valid()) {
        if ((ve = eval_binary(expr.op(), v0, v1)).is_valid()) {
          new_node = Const{*func_, ve};
        }
      }
    }
  }
  return new_node;
}

} // namespace onejit

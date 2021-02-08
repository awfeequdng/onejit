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

#include <onejit/binary.hpp>
#include <onejit/const.hpp>
#include <onejit/eval.hpp>
#include <onejit/func.hpp>
#include <onejit/node.hpp>
#include <onejit/optimizer.hpp>
#include <onejit/unary.hpp>

namespace onejit {

Optimizer::Optimizer() noexcept : func_{nullptr}, nodes_{}, opt_{None} {
}

Optimizer::~Optimizer() noexcept {
}

Node Optimizer::optimize(Func &func, Node node, Optimizer::Flag opt) noexcept {
  if (func && node && opt != None) {
    func_ = &func;
    nodes_.clear();
    opt_ = opt;
    optimize(node);
  }
  return node;
}

Optimizer::Result Optimizer::optimize(Node &node_inout) noexcept {
  Node node = node_inout;
  uint32_t n = node.children();
  if (n == 0 || node.type() >= LABEL) {
    return node.type() == CONST ? IsConst | IsSame : IsSame;
  }
  nodes_.clear();
  if (!nodes_.resize(n)) {
    return IsSame;
  }
  Result result = IsSame | IsConst;
  for (uint32_t i = 0; i < n; i++) {
    result = result & optimize(nodes_[i] = node.child(i));
  }

  if (result & IsConst) {
    Value v, v1, v2;
    if (Unary unary = node.is<Unary>()) {
      if (nodes_.size() == 1 //
          && (v1 = eval(nodes_[0].is<Expr>())).is_valid()) {
        v = eval_unary(unary.kind(), unary.op(), v1);
      }
    } else if (Binary binary = node.is<Binary>()) {
      if (nodes_.size() == 2                              //
          && (v1 = eval(nodes_[0].is<Expr>())).is_valid() //
          && (v2 = eval(nodes_[1].is<Expr>())).is_valid()) {
        v = eval_binary(binary.op(), v1, v2);
      }
    }
    if (v.is_valid() && (node = Const{*func_, v})) {
      node_inout = node;
      return IsConst;
    }
    result = result & ~IsConst;
  }
  if ((result & IsSame) || (node = Node::create_indirect(*func_, node.header(), nodes_))) {
    node_inout = node;
  }
  return result;
}

} // namespace onejit

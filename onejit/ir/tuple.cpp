/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * tuple.cpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/childrange.hpp>
#include <onejit/ir/functype.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/mem.hpp>
#include <onejit/ir/tuple.hpp>
#include <onestl/buffer.hpp>

namespace onejit {
namespace ir {

// ============================  Tuple  ====================================

uint16_t Tuple::compose_side_effects(OpN op, Nodes nodes) noexcept {
  uint16_t eff = op;
  for (size_t i = 0, n = nodes.size(); i < n; i++) {
    eff = eff | nodes[i].is<Expr>().side_effects();
  }
  return eff;
}

uint16_t Tuple::compose_side_effects(OpN op, const ChildRange &nodes) noexcept {
  uint16_t eff = op;
  for (size_t i = 0, n = nodes.size(); i < n; i++) {
    eff = eff | nodes[i].is<Expr>().side_effects();
  }
  return eff;
}

Node Tuple::create(Func &func, Kind kind, OpN op, Nodes nodes) noexcept {
  const uint16_t node_op = compose_side_effects(op, nodes);
  return Base::create_indirect(func,                         //
                               Header{TUPLE, kind, node_op}, //
                               nodes);
}

Node Tuple::create(Func &func, Kind kind, OpN op, const ChildRange &nodes) noexcept {
  const uint16_t node_op = compose_side_effects(op, nodes);
  return Base::create_indirect_from_ranges(func,                         //
                                           Header{TUPLE, kind, node_op}, //
                                           ChildRanges{&nodes, 1});
}

const Fmt &Tuple::format(const Fmt &fmt, Syntax syntax, size_t depth) const {
  if (type() == MEM) {
    // may happen, Mem is a subclass of Tuple
    return is<Mem>().format(fmt, syntax, depth);
  }
  OpN op = this->op();
  fmt << '(' << op;

  const bool is_call = op == CALL;
  // if op == CALL, skip child(0) i.e. FuncType
  for (size_t i = size_t(is_call), n = children(); i < n; i++) {
    Node node = child(i);
    if (node) {
      fmt << ' ';
      node.format(fmt, syntax, depth + 1);
    }
  }
  return fmt << ')';
}

} // namespace ir
} // namespace onejit

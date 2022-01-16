/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * address.cpp
 *
 *  Created on Feb 23, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/ir/childrange.hpp>
#include <onejit/ir/const.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/tuple.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/mir/address.hpp>
#include <onejit/mir/compiler.hpp>

namespace onejit {
namespace mir {

bool Address::insert(Compiler &comp, Node node) noexcept {
  Expr expr = node.is<Expr>();
  if (!expr) {
    return false;
  }

  switch (expr.type()) {
  case VAR:
    break;
  case TUPLE: {
    Tuple tuple = expr.is<Tuple>();
    if (index || tuple.op() != MUL || tuple.children() != 2) {
      break;
    }
    Expr x = tuple.arg(0);
    Value y = tuple.arg(1).is<Const>().val();
    if (!x || !y.is_valid() || y.kind().is_float()) {
      break;
    }
    Var my_index = comp.to_var(x);
    const int64_t my_scale = y.int64();
    // yval must be a power of two between 1 and 8
    if (my_index && my_scale > 0 && my_scale <= 8 && (my_scale & (my_scale - 1)) == 0) {
      index = my_index;
      scale = Scale(my_scale);
      return true;
    }
    break;
  }
  case CONST:
    if (offset == 0 && !expr.kind().is_float()) {
      offset = expr.is<Const>().val().int64();
    }
    break;
  default:
    break;
  } // switch

  // fallback
  if (!base) {
    base = comp.to_var(expr);
  } else if (!index) {
    index = comp.to_var(expr);
    scale = Scale1;
  }
  return true;
}

bool Address::insert(Compiler &comp, Nodes children) noexcept {
  for (Node node : children) {
    if (!insert(comp, node)) {
      return false;
    }
  }
  return true;
}

bool Address::insert(Compiler &comp, const ChildRange &children) noexcept {
  for (size_t i = 0, n = children.size(); i < n; i++) {
    if (!insert(comp, children[i])) {
      return false;
    }
  }
  return true;
}

} // namespace mir
} // namespace onejit

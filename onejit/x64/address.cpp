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
#include <onejit/x64/address.hpp>
#include <onejit/x64/compiler.hpp>

namespace onejit {
namespace x64 {

bool Address::insert(Compiler &comp, Node node) noexcept {
  if (Expr expr = node.is<Expr>()) {
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
      int64_t int_scale = y.int64();
      Scale my_scale;
      // yval must be a power of two between 1 and 8
      if (my_index && int_scale >= 0 && int_scale <= 0xff &&
          (my_scale = Scale(uint8_t(int_scale)))) {
        index = my_index;
        scale = my_scale;
        return true;
      }
      break;
    }
    case LABEL:
      if (!label) {
        label = expr.is<Label>();
        return true;
      }
      break;
    case CONST:
      if (offset == 0 && !expr.kind().is_float()) {
        int64_t delta = expr.is<Const>().val().int64();
        if (delta == int64_t(int32_t(delta))) {
          offset = int32_t(delta);
          return true;
        }
      }
      break;
    default:
      break;
    } // switch

    // fallback
    if (!base) {
      base = comp.to_var(expr);
      return true;
    } else if (!index) {
      index = comp.to_var(expr);
      scale = Scale1;
      return true;
    }
  }
  return false;
}

bool Address::insert(Compiler &comp, Exprs exprs) noexcept {
  for (Expr expr : exprs) {
    if (!insert(comp, expr)) {
      return false;
    }
  }
  return true;
}

bool Address::insert(Compiler &comp, const ChildRange &nodes) noexcept {
  for (uint32_t i = 0, n = nodes.size(); i < n; i++) {
    if (!insert(comp, nodes[i])) {
      return false;
    }
  }
  return true;
}

} // namespace x64
} // namespace onejit

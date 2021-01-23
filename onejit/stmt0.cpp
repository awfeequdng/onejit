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
 * stmt0.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/compiler.hpp>
#include <onejit/constexpr.hpp>
#include <onejit/func.hpp>
#include <onejit/stmt0.hpp>

namespace onejit {

// ============================  Stmt0  ========================================

Node Stmt0::compile(Compiler &comp, bool) const noexcept {
  switch (op()) {
  case BREAK:
    if (Label l = comp.label_break()) {
      comp.compile_add(comp.func().new_goto(l), false);
    } else {
      comp.error(*this, "misplaced Break");
    }
    break;
  case CONTINUE:
    if (Label l = comp.label_continue()) {
      comp.compile_add(comp.func().new_goto(l), false);
    } else {
      comp.error(*this, "misplaced Continue");
    }
    break;
  case FALLTHROUGH:
    comp.error(*this, "misplaced Fallthrough");
    break;
  case BAD:
  default:
    comp.error(*this, "bad Stmt0");
    break;
  }
  // all Stmt*::compile() must return VoidExpr
  return VoidExpr;
}

} // namespace onejit

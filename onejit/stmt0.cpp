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
 *  Created on Jan 18, 2020
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/stmt0.hpp>

namespace onejit {

// ============================  Stmt0  ========================================

Func &Stmt0::compile(Func &func) const noexcept {
  switch (op()) {
  case BREAK:
    if (Label l = func.label_break()) {
      return func.compile(func.new_goto(l));
    }
    return func.compile_error(*this, "misplaced Break");
  case CONTINUE:
    if (Label l = func.label_continue()) {
      return func.compile(func.new_goto(l));
    }
    return func.compile_error(*this, "misplaced Continue");
  case FALLTHROUGH:
    return func.compile_error(*this, "misplaced Fallthrough");
  case BAD:
  default:
    return func.compile_error(*this, "bad Stmt0");
  }
}

} // namespace onejit

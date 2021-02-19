/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
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
 * compiler.cpp
 *
 *  Created on Feb 19, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/compiler.hpp>
#include <onejit/func.hpp>
#include <onejit/x64/compiler.hpp>

namespace onejit {

Compiler &Compiler::x64(Func &func, Opt flags) noexcept {
  compile(func, flags);
  if (*this && error_.empty()) {
    // pass our internal buffers node_ and error_ to x64::Compiler
    onejit::x64::Compiler{}.compile(func, node_, error_, flags);
  }
  return *this;
}

namespace x64 {

Compiler::operator bool() const noexcept {
  return good_ && func_ && *func_;
}

Compiler &Compiler::compile(Func &func, Vector<Node> &node, Vector<Error> &error,
                            Opt flags) noexcept {
  node.clear();
  func_ = &func;
  node_ = &node;
  error_ = &error;
  good_ = bool(func);

  (void)flags;
  return *this;
}

Compiler &Compiler::error(const Node &where, Chars msg) noexcept {
  good_ = good_ && error_ && error_->append(Error{where, msg});
  return *this;
}

} // namespace x64
} // namespace onejit

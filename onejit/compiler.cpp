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
 * compiler.cpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/compiler.hpp>
#include <onejit/func.hpp>

namespace onejit {

Compiler::Compiler(Func &func) noexcept
    : func_{func}, break_{}, continue_{}, node_{}, error_{}, good_{func} {
}

Compiler::~Compiler() noexcept {
}

Compiler::operator bool() const noexcept {
  return good_ && func_;
}

Node Compiler::finish() noexcept {
  Node compiled;
  if (*this) {
    switch (node_.size()) {
    case 0:
      compiled = VoidExpr;
      break;
    case 1:
      compiled = node_[0];
      break;
    default:
      compiled = func_.new_block(node_);
      break;
    }
    if (compiled) {
      func_.set_compiled(compiled);
    }
  }
  return compiled;
}

Label Compiler::label_break() const noexcept {
  return break_.empty() ? Label{} : break_.end()[-1];
}

Label Compiler::label_continue() const noexcept {
  return continue_.empty() ? Label{} : continue_.end()[-1];
}

Compiler &Compiler::enter_case(Label l_break) noexcept {
  good_ = good_ && break_.append(l_break);
  return *this;
}

Compiler &Compiler::exit_case() noexcept {
  good_ = good_ && !break_.empty();
  if (good_) {
    break_.truncate(break_.size() - 1);
  }
  return *this;
}

Compiler &Compiler::enter_loop(Label l_break, Label l_continue) noexcept {
  good_ = good_ && break_.append(l_break) && continue_.append(l_continue);
  return *this;
}

Compiler &Compiler::exit_loop() noexcept {
  good_ = good_ && !break_.empty() && !continue_.empty();
  if (good_) {
    break_.truncate(break_.size() - 1);
    continue_.truncate(continue_.size() - 1);
  }
  return *this;
}

Compiler &Compiler::add(const Node &node) noexcept {
  good_ = good_ && node_.append(node);
  return *this;
}

Compiler &Compiler::error(const Node &where, Chars msg) noexcept {
  good_ = good_ && error_.append(Error{where, msg});
  return *this;
}

} // namespace onejit

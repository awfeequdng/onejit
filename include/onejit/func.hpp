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
 * func.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FUNC_HPP
#define ONEJIT_FUNC_HPP

#include <onejit/code.hpp>
#include <onejit/constexpr.hpp>
#include <onejit/fwd.hpp>
#include <onejit/varexpr.hpp>
#include <onejit/vector.hpp>

namespace onejit {

class Func {

public:
  explicit Func(Code *holder);

  ConstExpr new_const(const Const &c) {
    return ConstExpr::create(c, holder_);
  }

  VarExpr new_var(Kind kind);

  Func &add(const Node &node) {
    return add(node.offset_or_direct());
  }

private:
  Func &add(CodeItem item) {
    holder_->add(item);
    return *this;
  }

  Code *holder_;
  Vector<Var> regs_;
};

} // namespace onejit

#endif // ONEJIT_FUNC_HPP

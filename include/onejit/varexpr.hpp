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
 * varexpr.hpp
 *
 *  Created on Jan 15, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_VAREXPR_HPP
#define ONEJIT_VAREXPR_HPP

#include <onejit/node.hpp>
#include <onejit/var.hpp>

#include <iosfwd>

namespace onejit {

// an expression containing only a local variable or register.
class VarExpr : public Node {
  using Base = Node;

  friend class Func;

public:
  constexpr VarExpr() : Base{NodeHeader{VAR, Void, 0}, 0, nullptr} {
  }

  constexpr Type type() const {
    return VAR;
  }

  using Base::kind;

  constexpr uint16_t children() const {
    return 0;
  }

  Var var() const;

  VarId id() const {
    return var().id();
  }

  constexpr explicit operator bool() const {
    return kind() != Void;
  }

  constexpr bool is_direct() const {
    return code() == nullptr;
  }

private:
  constexpr VarExpr(NodeHeader header, CodeItem offset_or_data, Code *code)
      : Base{header, offset_or_data, code} {
  }

  static VarExpr create(Code *code, Var var);

  void add_to(Code *code) const {
    code->add(offset_or_data());
  }
};

std::ostream &operator<<(std::ostream &out, VarExpr v);

} // namespace onejit

#endif // ONEJIT_VAR_HPP

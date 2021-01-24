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
 *  Created on Jan 15, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_VAREXPR_HPP
#define ONEJIT_VAREXPR_HPP

#include <onejit/expr.hpp>
#include <onejit/var.hpp>

#include <iosfwd>

namespace onejit {

// an expression containing only a local variable or register.
class Var : public Expr {
  using Base = Expr;

  friend class Func;
  friend class Node;

public:
  /**
   * construct an invalid Var.
   * exists only to allow placing Var in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Var, use Func::new_var()
   */
  constexpr Var() noexcept : Base{VAR} {
  }

  static constexpr Type type() noexcept {
    return VAR;
  }

  using Base::kind;

  static constexpr uint32_t children() noexcept {
    return 0;
  }

  Variable var() const noexcept;

  VarId id() const noexcept {
    return var().id();
  }

private:
  // downcast Node to Const
  constexpr explicit Var(const Node &node) noexcept //
      : Base{Node{node.header(), node.offset_or_direct(),
                  node.offset_or_direct() & 2 ? nullptr : node.code()}} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == VAR;
  }

  static Var create(Variable var, Code *holder) noexcept;
};

std::ostream &operator<<(std::ostream &out, const Var &ve);

} // namespace onejit

#endif // ONEJIT_VAR_HPP

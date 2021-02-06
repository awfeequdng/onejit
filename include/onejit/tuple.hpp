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
 * tuple.hpp
 *
 *  Created on Jan 29, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_TUPLE_HPP
#define ONEJIT_TUPLE_HPP

#include <onejit/expr.hpp>
#include <onejit/op.hpp>
#include <onestl/view.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Tuple : public Expr {
  using Base = Expr;
  friend class Call;
  friend class Compiler;
  friend class Func;
  friend class Mem;
  friend class Node;

public:
  /**
   * construct an invalid Tuple.
   * exists only to allow placing Tuple in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Tuple, use one of the other constructors
   */
  constexpr Tuple() noexcept : Base{} {
  }

  constexpr OpN op() const noexcept {
    return OpN(Base::op());
  }

private:
  // used by Compiler::compile(Tuple)
  Tuple(Func &func, Kind kind, OpN op, Nodes nodes) noexcept //
      : Base{create(func, kind, op, nodes)} {
  }

  // downcast Node to Tuple
  constexpr explicit Tuple(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == TUPLE || t == MEM;
  }

  static Node create(Func &func, Kind kind, OpN op, Nodes nodes) noexcept;
};

const Fmt &operator<<(const Fmt &out, const Tuple &expr);

} // namespace onejit

#endif // ONEJIT_TUPLE_HPP

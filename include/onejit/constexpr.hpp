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
 * constexpr.hpp
 *
 *  Created on Jan 15, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_CONSTEXPR_HPP
#define ONEJIT_CONSTEXPR_HPP

#include <onejit/const.hpp>
#include <onejit/expr.hpp>

#include <iosfwd>

namespace onejit {

// an expression containing a constant
class ConstExpr : public Expr {
  using Base = Expr;

  friend class Func;
  friend class Node;

public:
  /**
   * construct an invalid ConstExpr.
   * exists only to allow placing ConstExpr in
containers
   * and similar uses that require a default constructor.
   *
   * to create a valid ConstExpr, use one of the other constructors
   * or Func::new_const()
   */
  constexpr ConstExpr() : Base{CONST} {
  }

  /* construct a Bool ConstExpr */
  constexpr explicit ConstExpr(bool val)
      : Base{Node{
            NodeHeader{CONST, Bool, 0},
            // must match Const::Direct::Direct(Const)
            1 | kBool << 1 | uint32_t(val) << 8,
            nullptr,
        }} {
  }

  /** construct a ConstExpr with specified kind and value */
  constexpr ConstExpr(Kind kind, uint16_t val)
      : Base{Node{
            NodeHeader{CONST, kind.nosimd(), 0},
            1 | (kind.val() & 0xF) << 1 |
                uint32_t(val) << 8, // must match Const::Direct::Direct(Const)
            nullptr,
        }} {
  }

  static constexpr Type type() {
    return CONST;
  }

  using Base::kind;

  static constexpr uint32_t children() {
    return 0;
  }

  Const constant() const;

private:
  // downcast Node to ConstExpr
  constexpr explicit ConstExpr(const Node &node) //
      : Base{Node{node.header(), node.offset_or_direct(),
                  node.offset_or_direct() & 1 ? nullptr : node.code()}} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) {
    return t == CONST;
  }

  static ConstExpr create(const Const &c, Code *holder);
};

constexpr const ConstExpr VoidExpr{Void, 0};
constexpr const ConstExpr TrueExpr{true};
constexpr const ConstExpr FalseExpr{false};

std::ostream &operator<<(std::ostream &out, const ConstExpr &ce);

} // namespace onejit

#endif // ONEJIT_VAR_HPP

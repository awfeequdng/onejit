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
  constexpr ConstExpr() noexcept : Base{CONST} {
  }

  /* construct a Bool ConstExpr */
  constexpr explicit ConstExpr(bool val) noexcept
      : Base{Node{
            NodeHeader{CONST, Bool, 0},
            Const::Direct{uint16_t(val), kBool},
            nullptr,
        }} {
  }

  /**
   * construct a ConstExpr with specified kind and value
   * only low 4 bits of kind are used, i.e. kind.simdn() is always set to 1
   */
  constexpr explicit ConstExpr(Kind kind, uint16_t val = 0) noexcept
      : Base{Node{
            NodeHeader{CONST, kind.nosimd(), 0},
            Const::Direct{val, kind.val()},
            nullptr,
        }} {
  }

  /**
   * construct a ConstExpr with specified kind and value.
   * only low 4 bits of kind are used, i.e. kind.simdn() is always set to 1
   */
  constexpr ConstExpr(Kind kind, int16_t val) noexcept
      : Base{Node{
            NodeHeader{CONST, kind.nosimd(), 0},
            Const::Direct{val, kind.val()},
            nullptr,
        }} {
  }

  static constexpr Type type() noexcept {
    return CONST;
  }

  using Base::kind;

  static constexpr uint32_t children() noexcept {
    return 0;
  }

  Const constant() const noexcept;

private:
  // downcast Node to ConstExpr
  constexpr explicit ConstExpr(const Node &node) noexcept //
      : Base{Node{node.header(), node.offset_or_direct(),
                  node.offset_or_direct() & 1 ? nullptr : node.code()}} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == CONST;
  }

  static ConstExpr create(const Const &c, Code *holder) noexcept;
};

constexpr const ConstExpr VoidExpr{Void};
constexpr const ConstExpr TrueExpr{true};
constexpr const ConstExpr FalseExpr{false};

std::ostream &operator<<(std::ostream &out, const ConstExpr &ce);

} // namespace onejit

#endif // ONEJIT_VAR_HPP

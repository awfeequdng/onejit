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
 * const.hpp
 *
 *  Created on Jan 15, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_CONSTEXPR_HPP
#define ONEJIT_CONSTEXPR_HPP

#include <onejit/expr.hpp>
#include <onejit/imm.hpp>

#include <iosfwd>

namespace onejit {

// an expression containing a constant
class Const : public Expr {
  using Base = Expr;

  friend class Func;
  friend class Node;

public:
  /**
   * construct an invalid Const.
   * exists only to allow placing Const in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Const, use one of the other constructors
   */
  constexpr Const() noexcept : Base{CONST} {
  }

  /* construct a Bool Const */
  constexpr explicit Const(bool val) noexcept
      : Base{Node{
            NodeHeader{CONST, Bool, 0},
            Imm::Direct{uint16_t(val), kBool},
            nullptr,
        }} {
  }

  /**
   * construct a Const with specified kind and value
   * only low 4 bits of kind are used, i.e. kind.simdn() is always set to 1
   */
  constexpr explicit Const(Kind kind, uint16_t val = 0) noexcept
      : Base{Node{
            NodeHeader{CONST, kind.nosimd(), 0},
            Imm::Direct{val, kind.val()},
            nullptr,
        }} {
  }

  /**
   * construct a Const with specified kind and value.
   * only low 4 bits of kind are used, i.e. kind.simdn() is always set to 1
   */
  constexpr Const(Kind kind, int16_t val) noexcept
      : Base{Node{
            NodeHeader{CONST, kind.nosimd(), 0},
            Imm::Direct{val, kind.val()},
            nullptr,
        }} {
  }

  /** construct a Const containing Imm{val} */
  template <class T>
  Const(Code *holder, T val) noexcept //
      : Base{create(holder, Imm{val})} {
  }

  /** construct a Const containing specified immediate value */
  Const(Code *holder, const Imm &imm) noexcept //
      : Base{create(holder, imm)} {
  }

  static constexpr Type type() noexcept {
    return CONST;
  }

  using Base::kind;

  static constexpr uint32_t children() noexcept {
    return 0;
  }

  Imm imm() const noexcept;

private:
  // downcast Node to Const
  constexpr explicit Const(const Node &node) noexcept //
      : Base{Node{node.header(), node.offset_or_direct(),
                  node.offset_or_direct() & 1 ? nullptr : node.code()}} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == CONST;
  }

  static Node create(Code *holder, const Imm &imm) noexcept;
};

constexpr const Const VoidExpr{Void};
constexpr const Const TrueExpr{true};
constexpr const Const FalseExpr{false};

// return a Const value = 0 with specified Kind (which must NOT be Bad)
constexpr inline Const Zero(Kind kind) noexcept {
  return Const{kind, uint16_t(0)};
}

// return a Const value = 1 with specified Kind (which must NOT be Bad or float)
constexpr inline Const One(Kind kind) noexcept {
  return Const{kind, uint16_t(1)};
}

// return a Const value = 2 with specified Kind (which must NOT be Bad or float)
constexpr inline Const Two(Kind kind) noexcept {
  return Const{kind, uint16_t(2)};
}

std::ostream &operator<<(std::ostream &out, const Const &c);

} // namespace onejit

#endif // ONEJIT_LOCAL_HPP

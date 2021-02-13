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

#ifndef ONEJIT_NODE_CONST_HPP
#define ONEJIT_NODE_CONST_HPP

#include <onejit/imm.hpp>
#include <onejit/node/expr.hpp>

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
  constexpr Const() noexcept : Base{} {
  }

  /* construct a Bool Const */
  constexpr explicit Const(bool val) noexcept
      : Base{Node{
            NodeHeader{CONST, Bool, 0},
            Imm::Direct{uint16_t(val), eBool},
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
  Const(Func &func, T val) noexcept //
      : Base{create(func, Imm{val})} {
  }

  /** construct a Const containing specified immediate value */
  Const(Func &func, const Value &value) noexcept //
      : Base{create(func, Imm{value})} {
  }

  /** construct a Const containing specified immediate value */
  Const(Func &func, const Imm &imm) noexcept //
      : Base{create(func, imm)} {
  }

  static constexpr Type type() noexcept {
    return CONST;
  }

  using Base::kind;

  static constexpr uint32_t children() noexcept {
    return 0;
  }

  // return immediate value of this constant expression
  Imm imm() const noexcept;

  // return immediate value of this constant expression
  Value val() const noexcept {
    return imm();
  }

  const Fmt &format(const Fmt &out, size_t depth = 0) const;

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

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    // no children
    return false;
  }

  static Node create(Func &func, const Imm &imm) noexcept;
};

constexpr const Const VoidConst{Void};
constexpr const Const TrueConst{true};
constexpr const Const FalseConst{false};

constexpr const Expr VoidExpr = VoidConst;
constexpr const Expr TrueExpr = TrueConst;
constexpr const Expr FalseExpr = FalseConst;

// return a Const value = 0 with specified Kind
// equivalent to Const{kind, uint16_t(0)}
constexpr inline Const Zero(Kind kind) noexcept {
  return Const{kind, uint16_t(0)};
}

// return a Const value = 1 with specified Kind
// equivalent to Const{func, Value{uint16_t(1)}.cast(kind)}
Const One(Func &func, Kind kind) noexcept;

// return a Const value = 2 with specified Kind
// equivalent to Const{func, Value{uint16_t(2)}.cast(kind)}
Const Two(Func &func, Kind kind) noexcept;

// return a Const value = -2 with specified Kind
// equivalent to Const{func, Value{int64_t(-1)}.cast(kind)}
Const MinusOne(Func &func, Kind kind) noexcept;

} // namespace onejit

#endif // ONEJIT_NODE_CONST_HPP

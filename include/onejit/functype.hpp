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
 * functype.hpp
 *
 *  Created on Jan 20, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FUNCTYPE_HPP
#define ONEJIT_FUNCTYPE_HPP

#include <onejit/node.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// function type signature
class FuncType : public Node {
  using Base = Node;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid FuncType.
   * exists only to allow placing FuncType in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid FuncType, use FuncType::create()
   */
  constexpr FuncType() noexcept : Base{NodeHeader{FTYPE, Bad, 0}, 0, nullptr} {
  }

  static FuncType create(uint8_t in_n, uint8_t out_n, const Kinds kinds, Code *holder) noexcept;

  /// \return number of parameters
  uint8_t in_n() const noexcept;
  /// \return number of results
  uint8_t out_n() const noexcept;

  /// \return kind of i-th parameter, or Bad if out-of-bounds
  Kind in(uint8_t i) const noexcept;
  /// \return kind of i-th result, or Bad if out-of-bounds
  Kind out(uint8_t i) const noexcept;

private:
  // downcast Node to FuncType
  constexpr explicit FuncType(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == FTYPE;
  }

  constexpr bool is_direct() const noexcept {
    return (offset_or_direct() & 0x4) != 0;
  }

  static constexpr Kind parse_direct_kind(uint32_t /*data*/) noexcept {
    return Void;
  }
};

std::ostream &operator<<(std::ostream &out, const FuncType &st);

} // namespace onejit

#endif // ONEJIT_FUNCTYPE_HPP

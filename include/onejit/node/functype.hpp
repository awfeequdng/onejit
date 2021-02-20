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
 *  Created on Jan 20, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_FUNCTYPE_HPP
#define ONEJIT_NODE_FUNCTYPE_HPP

#include <onejit/node/node.hpp>
#include <onestl/view.hpp>

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
   * to create a valid FuncType, use one of the other constructors
   */
  constexpr FuncType() noexcept : Base{} {
  }

  /// \pre params.size() must fit uint16_t, and result.size() must fit uint16_t
  FuncType(Code *holder, Kinds params, Kinds results) noexcept
      : Base{create(holder, params, results)} {
  }

  /// \pre params.size() must fit uint16_t, and result.size() must fit uint16_t
  FuncType(Code *holder, //
           std::initializer_list<Kind> params,
           std::initializer_list<Kind> results) noexcept
      : Base{create(holder, //
                    Kinds{params.begin(), params.size()}, Kinds{results.begin(), results.size()})} {
  }

  static constexpr Type type() noexcept {
    return FTYPE;
  }

  /// \return number of parameters
  uint16_t param_n() const noexcept;

  /// \return number of results
  constexpr uint16_t result_n() const noexcept {
    return Base::op();
  }

  /// \return kind of i-th parameter, or Bad if out-of-bounds
  Kind param(uint16_t i) const noexcept;
  /// \return kind of i-th result, or Bad if out-of-bounds
  Kind result(uint16_t i) const noexcept;

  const Fmt &format(const Fmt &out, Syntax syntax = Syntax::Default, size_t depth = 0) const;

private:
  // downcast Node to FuncType
  constexpr explicit FuncType(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == FTYPE;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    // children are parameters and results Kind
    return true;
  }

  static Node create(Code *holder, Kinds params, Kinds results) noexcept;
};

} // namespace onejit

#endif // ONEJIT_NODE_FUNCTYPE_HPP

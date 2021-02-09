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
 * name.hpp
 *
 *  Created on Jan 26, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_NAME_HPP
#define ONEJIT_NODE_NAME_HPP

#include <onejit/node/node.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// persistent string. used for name of function/globalconst/globalvar
class Name : public Node {
  using Base = Node;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Name.
   * exists only to allow placing Name in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Name, use one of the other constructors
   */
  constexpr Name() noexcept : Base{} {
  }

  /// \pre str.size() must fit uint16_t
  Name(Code *holder, Chars str) noexcept : Base{create(holder, str)} {
  }

  static constexpr Type type() noexcept {
    return NAME;
  }

  /// \return string size, in bytes
  constexpr uint16_t size() const noexcept {
    return Base::op();
  }

  /// \return reference to string contents inside Code* holder
  Chars chars() const noexcept;

  const Fmt &format(const Fmt &out, size_t depth = 0) const;

private:
  // downcast Node to Name
  constexpr explicit Name(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == NAME;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    // children contain the string
    return true;
  }

  static Node create(Code *holder, Chars str) noexcept;
};

} // namespace onejit

#endif // ONEJIT_NODE_NAME_HPP

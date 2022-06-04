/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * name.hpp
 *
 *  Created on Jan 26, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_NAME_HPP
#define ONEJIT_IR_NAME_HPP

#include <onejit/ir/node.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
// persistent string. used for name of function/globalconst/globalvar
class Name : public Node {
  using Base = Node;
  friend class Node;
  friend class ::onejit::Func;

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

  /// @return string size, in bytes
  constexpr uint16_t size() const noexcept {
    return Base::op();
  }

  /// @return reference to string contents inside Code* holder
  Chars chars() const noexcept;

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

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

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_NAME_HPP

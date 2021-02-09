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
 * label.hpp
 *
 *  Created on Jan 21, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_LABEL_HPP
#define ONEJIT_NODE_LABEL_HPP

#include <onejit/node/expr.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// jump destination. it contains an uint64_t that linker will set to
// absolute destination address
class Label : public Expr {
  using Base = Expr;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Label.
   * exists only to allow placing Label in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Label, use one of the other constructors
   */
  constexpr Label() noexcept : Base{} {
  }

  /** create a new label for local jumps within a Func */
  explicit Label(Func &func) noexcept;

  /** create a label pointing to an already compiled function */
  Label(Code *holder, uint64_t func_address) noexcept //
      : Base{create(holder, func_address, 0)} {
  }

  static constexpr Type type() noexcept {
    return LABEL;
  }

  constexpr uint16_t index() const noexcept {
    return Base::op();
  }

  // 0 if not resolved yet
  uint64_t address() const noexcept {
    return Base::uint64(offset_or_direct() + sizeof(CodeItem));
  }

  const Fmt &format(const Fmt &out, size_t depth = 0) const;

private:
  // downcast Node to Label
  constexpr explicit Label(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == LABEL;
  }

  static constexpr bool child_result_is_used(uint32_t /*i*/) noexcept {
    // no children
    return false;
  }

  /* create a new label. address == 0 means label is not resolved yet */
  static Label create(Code *holder, uint64_t address, uint16_t index) noexcept;
};

// position in Assembler that needs to be filled with Label relative address
struct Relocation {
  size_t pos;
  Label label;
};

} // namespace onejit

#endif // ONEJIT_NODE_LABEL_HPP

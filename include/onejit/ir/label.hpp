/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * label.hpp
 *
 *  Created on Jan 21, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_LABEL_HPP
#define ONEJIT_IR_LABEL_HPP

#include <onejit/ir/expr.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
// jump destination. it contains an uint64_t that linker will set to
// absolute destination address
class Label : public Expr {
  using Base = Expr;
  friend class Node;
  friend class ::onejit::Func;

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

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

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

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_LABEL_HPP

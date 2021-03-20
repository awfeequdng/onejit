/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * var.hpp
 *
 *  Created on Jan 15, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_VAR_HPP
#define ONEJIT_IR_VAR_HPP

#include <onejit/fmt.hpp>
#include <onejit/local.hpp>
#include <onejit/ir/expr.hpp>

namespace onejit {
namespace ir {

// an expression containing only a local variable or register.
class Var : public Expr {
  using Base = Expr;

  friend class ::onejit::Func;
  friend class Node;
  friend class VarHelper;

public:
  /**
   * construct an invalid Var.
   * exists only to allow placing Var in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Var, use one of the other constructors
   */
  constexpr Var() noexcept : Base{} {
  }

  // create a new local variable with specified kind
  Var(Func &func, Kind kind) noexcept;

  // create a Var for an existing local variable, which MUST be direct.
  // use with care!
  constexpr explicit Var(Local local) noexcept
      : Base{Node{Header{VAR, local.kind(), 0}, local.direct(), nullptr}} {
  }

  static constexpr Type type() noexcept {
    return VAR;
  }

  using Base::kind;

  static constexpr uint32_t children() noexcept {
    return 0;
  }

  Local local() const noexcept;

  Id id() const noexcept {
    return local().id();
  }

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

private:
  // downcast Node to Var
  constexpr explicit Var(const Node &node) noexcept //
      : Base{Node{node.header(), node.offset_or_direct(),
                  node.offset_or_direct() & 2 ? nullptr : node.code()}} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == VAR;
  }

  // called by Var{Func} -> Func::new_var()
  static Var create(Code *holder, Local local) noexcept;
};

class VarHelper {
public:
  // Var representing architectural flags register: EFLAGS on x86/x64, APSR on arm/arm64 ...
  static constexpr const Var Flags{Local{ArchFlags, Id{1}}};
};

// Var representing architectural flags register: EFLAGS on x86/x64, APSR on arm/arm64 ...
constexpr const Var FlagsVar = VarHelper::Flags;

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_VAR_HPP

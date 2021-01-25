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
 * call.hpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_CALLEXPR_HPP
#define ONEJIT_CALLEXPR_HPP

#include <onejit/expr.hpp>
#include <onejit/func.hpp>
#include <onejit/op.hpp>
#include <onestl/view.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Call : public Expr {
  using Base = Expr;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Call.
   * exists only to allow placing Call in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Call, use one of the other constructors
   */
  constexpr Call() noexcept : Base{CALL} {
  }

  // create a call to Func 'called'
  Call(Func &caller, const Func &called, std::initializer_list<Expr> args) noexcept
      : Base{create(caller, called.ftype(), called.label(), Exprs{args.begin(), args.size()})} {
  }

  // create a call to Func 'called'
  Call(Func &caller, const Func &called, Exprs args) noexcept
      : Base{create(caller, called.ftype(), called.label(), args)} {
  }

  // create a call to an arbitrary function, for example a Func or an already compiled C function
  Call(Func &caller, const FuncType &ftype, const Label &flabel,
       std::initializer_list<Expr> args) noexcept
      : Base{create(caller, ftype, flabel, Exprs{args.begin(), args.size()})} {
  }

  // create a call to an arbitrary function, for example a Func or an already compiled C function
  Call(Func &caller, const FuncType &ftype, const Label &flabel, Exprs args) noexcept
      : Base{create(caller, ftype, flabel, args)} {
  }

  static constexpr OpN op() noexcept {
    return CALL_OP;
  }

  // shortcut for child(0).is<FuncType>()
  FuncType ftype() const noexcept;

  // shortcut for child(1).is<Label>()
  Label label() const noexcept;

  // shortcut for child(i+2).is<Expr>()
  Expr arg(uint32_t i) const noexcept;

private:
  // downcast Node to Call
  constexpr explicit Call(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == CALL;
  }

  static Node create(Func &caller, const FuncType &ftype, const Label &flabel, Exprs args) noexcept;
};

std::ostream &operator<<(std::ostream &out, const Call &expr);

} // namespace onejit

#endif // ONEJIT_CALLEXPR_HPP

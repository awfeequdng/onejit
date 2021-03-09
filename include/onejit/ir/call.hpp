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

#ifndef ONEJIT_IR_CALL_HPP
#define ONEJIT_IR_CALL_HPP

#include <onejit/func.hpp>
#include <onejit/ir/tuple.hpp>
#include <onestl/view.hpp>

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
class Call : public Tuple {
  using Base = Tuple;
  friend class Node;
  friend class ::onejit::Func;

public:
  /**
   * construct an invalid Call.
   * exists only to allow placing Call in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Call, use one of the other constructors
   */
  constexpr Call() noexcept : Base{} {
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
    return CALL;
  }

  // shortcut for child_is<FuncType>(0)
  FuncType ftype() const noexcept {
    return child_is<FuncType>(0);
  }

  // shortcut for child_is<Label>(1)
  Label label() const noexcept {
    return child_is<Label>(1);
  }

  // shortcut for child_is<Expr>(i+2)
  Expr arg(uint32_t i) const noexcept;

private:
  // downcast Node to Call
  constexpr explicit Call(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == CALL;
  }

  static Node create(Func &caller, const FuncType &ftype, const Label &flabel, Exprs args) noexcept;
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_CALL_HPP

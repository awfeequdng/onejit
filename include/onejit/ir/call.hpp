/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
  Call(Func &caller, const FuncHeader &called, std::initializer_list<Expr> args) noexcept
      : Base{create(caller, called.ftype(), called.address(), Exprs{args.begin(), args.size()})} {
  }

  // create a call to Func 'called'
  Call(Func &caller, const FuncHeader &called, Exprs args) noexcept
      : Base{create(caller, called.ftype(), called.address(), args)} {
  }

  static constexpr OpN op() noexcept {
    return CALL;
  }

  // shortcut for child_is<FuncType>(0)
  FuncType ftype() const noexcept {
    return child_is<FuncType>(0);
  }

  // shortcut for child_is<Expr>(1)
  Expr address() const noexcept {
    return child_is<Expr>(1);
  }

  FuncHeader fheader() const noexcept {
    return FuncHeader{Name{}, ftype(), address()};
  }

  // shortcut for child_is<Expr>(i+2)
  Expr arg(uint32_t i) const noexcept;

private:
  // downcast Node to Call
  constexpr explicit Call(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return OpN(op) == CALL;
  }

  static Node create(Func &caller, const FuncType &ftype, const Expr &address, Exprs args) noexcept;
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_CALL_HPP

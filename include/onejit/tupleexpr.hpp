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
 * tupleexpr.hpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_TUPLEEXPR_HPP
#define ONEJIT_TUPLEEXPR_HPP

#include <onejit/expr.hpp>
#include <onejit/op.hpp>
#include <onestl/view.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// base class of CallExpr, AssignStmt, ReturnStmt
class TupleExpr : public Expr {
  using Base = Expr;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid TupleExpr.
   * exists only to allow placing TupleExpr in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid TupleExpr, use Func::new_call(), Func::new_assign()
   * or Func::new_return()
   */
  constexpr TupleExpr() noexcept : Base{TUPLE} {
  }

  constexpr OpN op() const noexcept {
    return OpN(Base::op());
  }

protected:
  /* construct an invalid TupleExpr */
  constexpr explicit TupleExpr(OpN op) noexcept : Base{TUPLE, op} {
  }

  // downcast Node to TupleExpr
  constexpr explicit TupleExpr(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == TUPLE;
  }
};

std::ostream &operator<<(std::ostream &out, const TupleExpr &tuple);

////////////////////////////////////////////////////////////////////////////////
class CallExpr : public TupleExpr {
  using Base = TupleExpr;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid CallExpr.
   * exists only to allow placing CallExpr in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid CallExpr, use Func::new_call()
   */
  constexpr CallExpr() noexcept : Base{CALL} {
  }

  static constexpr OpN op() noexcept {
    return CALL;
  }

  // shortcut for child(0).is<FuncType>()
  FuncType ftype() const noexcept;

  // shortcut for child(1).is<Label>()
  Label label() const noexcept;

  // shortcut for children() - 2
  uint32_t arg_n() const noexcept {
    uint32_t n = children();
    return n >= 2 ? n - 2 : 0;
  }

  // shortcut for child(i + 2).is<Expr>()
  Expr arg(uint32_t i) const noexcept;

  CallExpr compile(Compiler &comp) const noexcept;

private:
  // downcast Node to CallExpr
  constexpr explicit CallExpr(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == CALL;
  }

  static CallExpr create(const FuncType &ftype, const Label &flabel, Exprs args,
                         Code *holder) noexcept;
};

} // namespace onejit

#endif // ONEJIT_TUPLEEXPR_HPP

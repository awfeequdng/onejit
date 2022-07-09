/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * eval.hpp
 *
 *  Created on Feb 07, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_EVAL_HPP
#define ONEJIT_EVAL_HPP

#include <initializer_list>
#include <onejit/fwd.hpp>

namespace onejit {

bool is_const(Expr expr) noexcept;

// evaluate a constant expression
Value eval_const(Expr expr) noexcept;

Value eval_unary_op(Kind kind, Op1 op, Value x) noexcept;
Value eval_binary_op(Op2 op, Value x, Value y) noexcept;
Value eval_tuple_op(Kind kind, OpN op, Values vs) noexcept;
Value eval_tuple_op(Kind kind, OpN op, std::initializer_list<Value> vs) noexcept;

} // namespace onejit

#endif // ONEJIT_EVAL_HPP

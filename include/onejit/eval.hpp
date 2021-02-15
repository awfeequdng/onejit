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
Value eval(Expr expr) noexcept;
Value eval_unary(Kind kind, Op1 op, Value x) noexcept;
Value eval_binary(Op2 op, Value x, Value y) noexcept;
Value eval_tuple(Kind kind, OpN op, Values vs) noexcept;
Value eval_tuple(Kind kind, OpN op, std::initializer_list<Value> vs) noexcept;

} // namespace onejit

#endif // ONEJIT_EVAL_HPP

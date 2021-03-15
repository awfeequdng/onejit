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
 * func.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FUNC_HPP
#define ONEJIT_FUNC_HPP

#include <onejit/archid.hpp>
#include <onejit/code.hpp>
#include <onejit/funcheader.hpp>
#include <onejit/ir/functype.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/name.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/op.hpp>
#include <onejit/x64/fwd.hpp>
#include <onestl/array.hpp>
#include <onestl/chars.hpp>
#include <onestl/string.hpp>

namespace onejit {

class Func : private FuncHeader {
  using Base = FuncHeader;

  friend class Compiler;
  friend class ir::Label;
  friend class ir::Var;
  friend class x64::Compiler;

public:
  /**
   * construct an invalid Func.
   * exists only to allow placing Func in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Func, use one of the other constructors
   */
  Func() noexcept;

  Func(Code *holder, Name name, FuncType ftype) noexcept : Func{} {
    reset(holder, name, ftype);
  }

  Func(Func &&other) noexcept = default;
  Func &operator=(Func &&other) noexcept = default;

  ~Func() noexcept;

  constexpr Code *code() const noexcept {
    return holder_;
  }

  constexpr explicit operator bool() const noexcept {
    return holder_ && *holder_;
  }

  using Base::ftype;
  using Base::name;
  using Base::param_n;
  using Base::result_n;

  // get FuncHeader
  constexpr const FuncHeader &fheader() const noexcept {
    return *this;
  }

  // get function address. the returned label may be not resolved yet.
  constexpr Label address() const noexcept {
    return labels_[0];
  }

  /// \return i-th function parameter
  constexpr Var param(uint16_t i) const noexcept {
    return params()[i];
  }

  /// \return i-th function result
  constexpr Var result(uint16_t i) const noexcept {
    return results()[i];
  }

  /// \return function parameters
  constexpr Vars params() const noexcept {
    return vars_.view(0, param_n());
  }

  /// \return function results
  constexpr Vars results() const noexcept {
    return vars_.view(param_n(), param_n() + result_n());
  }

  // reinitialize Func
  Func &reset(Code *holder, Name name, FuncType ftype) noexcept;

  //////////////////////////////////////////////////////////////////////////////

  constexpr Node get_body() const noexcept {
    return body_;
  }

  Func &set_body(const Node &body) noexcept {
    body_var_n_ = vars_.size();
    body_ = body;
    return *this;
  }

  Node get_compiled(ArchId archid) const noexcept;

  Func &set_compiled(ArchId archid, const Node &compiled) noexcept;

private:
  // create a new local label, used for jumps within the function
  Label new_label() noexcept;

  // create a new local variable. called by Var{Func&, Kind}
  // and internally calls Var::create()
  Var new_var(Kind kind) noexcept;

  // return all local variables
  constexpr Vars vars() const noexcept {
    return vars_;
  }

private:
  Code *holder_;
  uint32_t body_var_n_;     // # local vars used by body_
  uint32_t compiled_var_n_; // # local vars used by compiled_[NOARCH]

  Array<Var> vars_;
  Array<Label> labels_;
  Node body_;
  Node compiled_[ARCHID_N]; // compiled code. index is archid
};

} // namespace onejit

#endif // ONEJIT_FUNC_HPP

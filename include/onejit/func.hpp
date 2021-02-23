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
#include <onejit/node/functype.hpp>
#include <onejit/node/label.hpp>
#include <onejit/node/name.hpp>
#include <onejit/node/var.hpp>
#include <onejit/op.hpp>
#include <onestl/chars.hpp>
#include <onestl/string.hpp>
#include <onestl/vector.hpp>

namespace onejit {

class Func {
  friend class Compiler;
  friend class Label;
  friend class Var;

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

  // get function name
  constexpr Name name() const noexcept {
    return name_;
  }

  // reinitialize Func
  Func &reset(Code *holder, Name name, FuncType ftype) noexcept;

  // convert Func to Label.
  Label label() const noexcept;

  // get function type
  constexpr FuncType ftype() const noexcept {
    return ftype_;
  }

  // get number of params
  constexpr uint16_t param_n() const noexcept {
    return param_n_;
  }
  // get number of results
  constexpr uint16_t result_n() const noexcept {
    return result_n_;
  }

  /// \return i-th parameter, or Var{} if out-of-bounds
  Var param(uint16_t i) const noexcept;
  /// \return i-th result, or Var{} if out-of-bounds
  Var result(uint16_t i) const noexcept;

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
  uint16_t param_n_;
  uint16_t result_n_;
  uint32_t body_var_n_;     // # local vars used by body_
  uint32_t compiled_var_n_; // # local vars used by compiled_[NOARCH]

  FuncType ftype_;
  Vector<Var> vars_;
  Vector<Label> labels_;
  Name name_;
  Node body_;
  Node compiled_[ARCHID_N]; // compiled code. index is archid
};

} // namespace onejit

#endif // ONEJIT_FUNC_HPP

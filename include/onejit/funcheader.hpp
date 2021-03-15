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
 * funcheader.hpp
 *
 *  Created on Mar 15, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FUNCHEADER_HPP
#define ONEJIT_FUNCHEADER_HPP

#include <onejit/ir/functype.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/name.hpp>
#include <onestl/string.hpp>

namespace onejit {

/**
 * Function header: only contains function's name, prototype and
 * (possibly not yet resolved) address - not its body.
 *
 * Used to call a function.
 */
class FuncHeader {

public:
  /**
   * construct an invalid FuncHeader.
   * exists only to allow placing FuncHeader in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid FuncHeader, use one of the other constructors
   */
  FuncHeader() noexcept;

  // address can be a label (either resolved or unresolved)
  // or an expression that computes the function address (i.e. a function pointer)
  FuncHeader(Name name, FuncType ftype, Expr address) noexcept : FuncHeader{} {
    reset(name, ftype, address);
  }

  FuncHeader(FuncHeader &&) noexcept = default;
  FuncHeader(const FuncHeader &) noexcept = default;

  ~FuncHeader() noexcept = default;

  FuncHeader &operator=(FuncHeader &&) noexcept = default;
  FuncHeader &operator=(const FuncHeader &) noexcept = default;

  // get function name
  constexpr Name name() const noexcept {
    return name_;
  }

  // get function type
  constexpr FuncType ftype() const noexcept {
    return ftype_;
  }

  // get function address: either a resolved or unresolved label,
  // or an expression that computes the function address (i.e. a function pointer)
  constexpr Expr address() const noexcept {
    return address_;
  }

  // reinitialize this FuncHeader
  FuncHeader &reset(Name name, FuncType ftype, Expr address) noexcept;

  // get number of params
  constexpr uint16_t param_n() const noexcept {
    return param_n_;
  }
  // get number of results
  constexpr uint16_t result_n() const noexcept {
    return result_n_;
  }

  /// \return kind of i-th parameter, or Bad if out-of-bounds
  Kind param(uint16_t i) const noexcept {
    return ftype_.param(i);
  }

  /// \return kind of i-th result, or Bad if out-of-bounds
  Kind result(uint16_t i) const noexcept {
    return ftype_.result(i);
  }

private:
  Name name_;
  FuncType ftype_;
  Expr address_;
  uint16_t param_n_;
  uint16_t result_n_;
};

} // namespace onejit

#endif // ONEJIT_FUNCHEADER_HPP

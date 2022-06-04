/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * functype.cpp
 *
 *  Created on Jan 21, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/ir/functype.hpp>
#include <onejit/math.hpp>
#include <onestl/chars.hpp>

namespace onejit {
namespace ir {

/// @return number of parameters
uint16_t FuncType::param_n() const noexcept {
  return sub_uint16(children(), result_n());
}

/// @return kind of i-th parameter, or Bad if fmt-of-bounds
Kind FuncType::param(uint16_t i) const noexcept {
  return child(i + result_n()).kind();
}

/// @return kind of i-th result, or Bad if fmt-of-bounds
Kind FuncType::result(uint16_t i) const noexcept {
  return i >= result_n() ? Bad : child(i).kind();
}

// param_n must fit uint16_t, and result_n must fit uint16_t
static constexpr bool functype_can_represent(size_t param_n, size_t result_n) {
  return (param_n | result_n) <= 0xFFFF;
}

Node FuncType::create(Code *holder, Kinds params, Kinds results) noexcept {
  const size_t param_n = params.size();
  const size_t result_n = results.size();
  while (holder && functype_can_represent(param_n, result_n)) {
    const Header header{FTYPE, results.empty() ? Void : results[0], uint16_t(result_n)};
    CodeItem offset = holder->length();

    // put results first, as expected by param(uint32_t) and result(uint32_t)
    if (holder->add(header) && holder->add_uint32(param_n + result_n) //
        && holder->add(results) && holder->add(params)) {

      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Node{};
}

const Fmt &FuncType::format(const Fmt &fmt, Syntax /*syntax*/, size_t /*depth*/) const {
  fmt << '(' << type() << " (";
  for (size_t i = 0, n = param_n(); i < n; i++) {
    if (i) {
      fmt << ' ';
    }
    fmt << param(i);
  }
  fmt << ") -> (";
  for (size_t i = 0, n = result_n(); i < n; i++) {
    if (i) {
      fmt << ' ';
    }
    fmt << result(i);
  }
  return fmt << "))";
}

} // namespace ir
} // namespace onejit

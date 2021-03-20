/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * error.hpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ERROR_HPP
#define ONEJIT_ERROR_HPP

#include <onejit/fwd.hpp>
#include <onejit/ir/node.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// a compile error, with Node that caused it and error message
class Error {
public:
  constexpr Error() noexcept : where_{}, msg_{} {
  }

  constexpr Error(Node where, Chars msg) noexcept : where_{where}, msg_{msg} {
  }

  ~Error() noexcept = default;

  constexpr Node where() const noexcept {
    return where_;
  }

  constexpr Chars msg() const noexcept {
    return msg_;
  }

private:
  Node where_;
  Chars msg_;
};

const Fmt &operator<<(const Fmt &fmt, const Error &error);
const Fmt &operator<<(const Fmt &fmt, const Errors &errors);
const Fmt &operator<<(const Fmt &fmt, const CRange<Error> &errors);

} // namespace onejit

#endif // ONEJIT_ERROR_HPP

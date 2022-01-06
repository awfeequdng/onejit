/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * opstmt4.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPSTMT4_HPP
#define ONEJIT_OPSTMT4_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint16_t

namespace onejit {

enum OpStmt4 : uint16_t {
  BAD_ST4 = 0,
  FOR = 1,

  // numeric values of the OpStmt4 enum constants below this line MAY CHANGE WITHOUT WARNING

};

constexpr OpStmt4 operator+(OpStmt4 op, int delta) noexcept {
  return OpStmt4(int(op) + delta);
}
constexpr OpStmt4 operator-(OpStmt4 op, int delta) noexcept {
  return OpStmt4(int(op) - delta);
}

const Chars to_string(OpStmt4 op) noexcept;

const Fmt &operator<<(const Fmt &fmt, OpStmt4 op);

} // namespace onejit

#endif // ONEJIT_OPSTMT4_HPP

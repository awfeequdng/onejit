/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * allow.hpp
 *
 *  Created on Feb 17, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_ALLOW_HPP
#define ONEJIT_IR_ALLOW_HPP

#include <cstdint>

namespace onejit {
namespace ir {

// Flags passed to Node::deep_equal(), Node::deep_compare(), Node::deep_pure()
// Intentionally uses the same numerical values as enum Check.
enum Allow : uint8_t {
  AllowNone = 0,
  AllowDivision = 1 << 0,  // including possible division by zero
  AllowMemAccess = 1 << 1, // including dereference of possibly null pointer
  AllowCall = 1 << 2,
  AllowAll = 0xff,
};

constexpr inline Allow operator~(Allow a) noexcept {
  return Allow(~unsigned(a));
}

constexpr inline Allow operator&(Allow a, Allow b) noexcept {
  return Allow(unsigned(a) & unsigned(b));
}
constexpr inline Allow operator|(Allow a, Allow b) noexcept {
  return Allow(unsigned(a) | unsigned(b));
}
constexpr inline Allow operator^(Allow a, Allow b) noexcept {
  return Allow(unsigned(a) ^ unsigned(b));
}

inline Allow &operator&=(Allow &a, Allow b) noexcept {
  return a = a & b;
}
inline Allow &operator|=(Allow &a, Allow b) noexcept {
  return a = a | b;
}
inline Allow &operator^=(Allow &a, Allow b) noexcept {
  return a = a ^ b;
}

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_ALLOW_HPP

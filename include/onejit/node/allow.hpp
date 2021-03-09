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
 * allow.hpp
 *
 *  Created on Feb 17, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_ALLOW_HPP
#define ONEJIT_NODE_ALLOW_HPP

#include <cstdint>

namespace onejit {
namespace node {

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

} // namespace node
} // namespace onejit

#endif // ONEJIT_NODE_ALLOW_HPP

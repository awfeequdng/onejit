/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
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
 * regid.hpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_REGID_HPP
#define ONEJIT_X64_REGID_HPP

#include <onejit/archid.hpp>

namespace onejit {
namespace x64 {

enum RegId : uint32_t {
  // use onejit::Id{256...511}
  // 0 is reserved for onejit::NOID
  // 512...767 are reserved for arm64::RegId
  RAX = uint32_t(eArchId::X64) << 8,
  RCX,
  RDX,
  RBX,
  RSP,
  RBP,
  RSI,
  RDI,
  R8,
  R9,
  R10,
  R11,
  R12,
  R13,
  R14,
  R15,
  RIP = RBP + 16, // rlo(RIP), rhi(RIP) must be == rlo(RBP), rhi(RBP)
  XMM0 = RAX + 32,
  XMM1,
  XMM2,
  XMM3,
  XMM4,
  XMM5,
  XMM6,
  XMM7,
  XMM8,
  XMM9,
  XMM10,
  XMM11,
  XMM12,
  XMM13,
  XMM14,
  XMM15,
  XMM16, // XMM16..XMM31 require [CPUID AVX512F]
  XMM17,
  XMM18,
  XMM19,
  XMM20,
  XMM21,
  XMM22,
  XMM23,
  XMM24,
  XMM25,
  XMM26,
  XMM27,
  XMM28,
  XMM29,
  XMM30,
  XMM31,
};

constexpr inline RegId operator+(RegId id, int delta) noexcept {
  return RegId(int(id) + delta);
}

constexpr inline RegId operator-(RegId id, int delta) noexcept {
  return RegId(int(id) - delta);
}

constexpr inline uint8_t rlo(RegId id) noexcept {
  return id & 0x7;
}

constexpr inline uint8_t rhi(RegId id) noexcept {
  return (id >> 3) & 0x1;
}

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_REGID_HPP

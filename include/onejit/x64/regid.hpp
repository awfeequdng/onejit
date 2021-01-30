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

#include <cstdint> // uint*_t

namespace onejit {
namespace x64 {

enum RegId : uint16_t {
  // use interval 256..511 of onejit::Id{}
  // 0 is reserved for onejit::NOID
  RAX = 0x100,
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
  RIP = 0x110,
  XMM0 = 0x120,
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
  ZMM16,
  ZMM17,
  ZMM18,
  ZMM19,
  ZMM20,
  ZMM21,
  ZMM22,
  ZMM23,
  ZMM24,
  ZMM25,
  ZMM26,
  ZMM27,
  ZMM28,
  ZMM29,
  ZMM30,
  ZMM31,
};

constexpr inline uint8_t rlo(RegId r) noexcept {
  return r & 0x7;
}

constexpr inline uint8_t rhi(RegId r) noexcept {
  return (r >> 3) & 0x1;
}

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_REGID_HPP

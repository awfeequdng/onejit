/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * regid.hpp
 *
 *  Created on Feb 05, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_ARM64_REGID_HPP
#define ONEJIT_ARM64_REGID_HPP

#include <onejit/archid.hpp>

namespace onejit {
namespace arm64 {

enum RegId : uint32_t {
  // use onejit::Id{512...1023}
  // 0 is reserved for onejit::NOID
  // 256...511 are reserved for x64::RegId
  X0 = uint32_t(ArchId::ARM64) << 8,
  X1,
  X2,
  X3,
  X4,
  X5,
  X6,
  X7,
  X8,
  X9,
  X10,
  X11,
  X12,
  X13,
  X14,
  X15,
  X16,
  X17,
  X18,
  X19,
  X20,
  X21,
  X22,
  X23,
  X24,
  X25,
  X26,
  X27,
  X28,
  X29,
  X30,
  X31,
  FP = X29,
  LR = X30,
  SP = X31,
  ZR = X31,

  V0,
  V1,
  V2,
  V3,
  V4,
  V5,
  V6,
  V7,
  V8,
  V9,
  V10,
  V11,
  V12,
  V13,
  V14,
  V15,
  V16,
  V17,
  V18,
  V19,
  V20,
  V21,
  V22,
  V23,
  V24,
  V25,
  V26,
  V27,
  V28,
  V29,
  V30,
  V31,
};

constexpr inline RegId operator+(RegId id, int delta) noexcept {
  return RegId(int(id) + delta);
}

constexpr inline RegId operator-(RegId id, int delta) noexcept {
  return RegId(int(id) - delta);
}

} // namespace arm64
} // namespace onejit

#endif // ONEJIT_ARM64_REGID_HPP

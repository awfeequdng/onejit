/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * reg.hpp
 *
 *  Created on Jan 30, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_REG_HPP
#define ONEJIT_X64_REG_HPP

#include <onejit/local.hpp>
#include <onejit/x64/regid.hpp>

namespace onejit {
namespace x64 {

class Reg : public Local {
  using Base = Local;

public:
  // create an invalid Reg.
  constexpr Reg() noexcept : Base{} {
  }

  // downcast Local to Reg
  constexpr explicit Reg(const Local &local) noexcept : Base{local} {
  }

  /* create a Reg with specified Kind and Id. */
  constexpr Reg(Kind kind, Id id) noexcept //
      : Base{kind, id} {
  }

  /* create a Reg with specified Kind and RegId. */
  constexpr Reg(Kind kind, RegId id) noexcept //
      : Base{kind, Id{uint32_t(id)}} {
  }

  constexpr RegId reg_id() const noexcept {
    return RegId(Base::id().val());
  }

}; // class Reg

constexpr inline uint8_t rlo(Reg reg) noexcept {
  return rlo(reg.reg_id());
}

inline uint8_t rhi(Reg reg) noexcept {
  return rhi(reg.reg_id());
}

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_REG_HPP

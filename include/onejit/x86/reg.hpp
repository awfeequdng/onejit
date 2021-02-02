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
 * reg.hpp
 *
 *  Created on Jan 30, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X86_REG_HPP
#define ONEJIT_X86_REG_HPP

#include <onejit/local.hpp>
#include <onejit/x86/regid.hpp>

namespace onejit {
namespace x86 {

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

} // namespace x86
} // namespace onejit

#endif // ONEJIT_X86_REG_HPP

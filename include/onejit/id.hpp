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
 * id.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ID_HPP
#define ONEJIT_ID_HPP

#include <onejit/fwd.hpp>
#include <onejit/kind.hpp>
#include <onejit/x64/fwd.hpp>

namespace onejit {

// index of a local variable or register.
class Id {
  friend class Func;
  friend class Local;
  friend class Var;
  friend class VarHelper;
  friend class x64::Reg;

public:
  enum {
    FIRST = 0x1000,
  };

  constexpr Id() noexcept : val_{} {
  }

  constexpr uint32_t val() const noexcept {
    return val_;
  }

  const Fmt &format(const Fmt &fmt, Kind kind) const {
    return format(fmt, *this, kind);
  }

private:
  constexpr explicit Id(uint32_t val) noexcept : val_{val & 0xFFFFFF} {
  }

  static const Fmt &format(const Fmt &fmt, Id id, Kind kind);

  // only 24 bits are used
  uint32_t val_;
};

constexpr inline bool operator==(Id a, Id b) noexcept {
  return a.val() == b.val();
}

constexpr inline bool operator!=(Id a, Id b) noexcept {
  return a.val() != b.val();
}

constexpr const Id NOID{};

inline const Fmt &operator<<(const Fmt &fmt, Id id) {
  return id.format(fmt, Ptr);
}

} // namespace onejit

#endif // ONEJIT_ID_HPP

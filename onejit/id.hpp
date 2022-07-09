/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
  friend class ir::Var;
  friend class ir::VarHelper;
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

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
 * var.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_VAR_HPP
#define ONEJIT_VAR_HPP

#include <onejit/node.hpp>

#include <iosfwd>

namespace onejit {

class VarId {
  friend class Func;
  friend union Var;
  friend class VarExpr;

public:
  constexpr VarId() noexcept : val_{} {
  }

  constexpr uint32_t val() const noexcept {
    return val_;
  }

private:
  constexpr explicit VarId(uint32_t val) noexcept : val_{val & 0xFFFFFF} {
  }

  // at most 24 bits can be used
  uint32_t val_;
};

constexpr inline bool operator==(VarId a, VarId b) noexcept {
  return a.val() == b.val();
}

constexpr inline bool operator!=(VarId a, VarId b) noexcept {
  return a.val() != b.val();
}

constexpr const VarId NOID = VarId{};

std::ostream &operator<<(std::ostream &out, VarId id);

////////////////////////////////////////////////////////////////////////////////

// a local variable or register.
union Var {
  friend class Func;
  friend class Node;
  friend class VarExpr;
  friend class Test;

public:
  constexpr Var() noexcept : val_{kBad} {
  }

  constexpr Type type() const noexcept {
    return VAR;
  }

  constexpr Kind kind() const noexcept {
    return Kind{uint8_t(val_)};
  }

  constexpr VarId id() const noexcept {
    return VarId{val_ >> 8};
  }

  constexpr explicit operator bool() const noexcept {
    return uint8_t(val_) != kBad;
  }

private:
  constexpr explicit Var(uint32_t val) noexcept : val_{val} {
  }

  constexpr Var(Kind kind, VarId id) noexcept : val_{kind.val() | id.val() << 8} {
  }

  constexpr bool is_direct() const noexcept {
    return (val_ >> 29) == 0;
  }

  // useful only if is_direct() returns true
  constexpr uint32_t direct() const noexcept {
    return 0x2 | val_ << 3;
  }
  static constexpr Kind parse_direct_kind(uint32_t data) noexcept {
    return Kind(data >> 3);
  }
  static constexpr Var parse_direct(uint32_t data) noexcept {
    return Var{data >> 3};
  }

  // useful only if is_direct() returns false
  constexpr uint32_t indirect() const noexcept {
    return val_ >> 8;
  }
  static constexpr Var parse_indirect(Kind kind, uint32_t data) noexcept {
    return Var{kind.val() | data << 8};
  }

  uint32_t val_;
  struct { // for debug purposes
    eKind ekind;
    uint8_t id[3];
  } u_;
};

constexpr inline bool operator==(Var a, Var b) noexcept {
  return a.kind() == b.kind() && a.id() == b.id();
}

constexpr inline bool operator!=(Var a, Var b) noexcept {
  return a.kind() != b.kind() || a.id() != b.id();
}

std::ostream &operator<<(std::ostream &out, Var var);

} // namespace onejit

#endif // ONEJIT_VAR_HPP

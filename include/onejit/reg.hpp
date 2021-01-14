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
 * reg.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_REG_HPP
#define ONEJIT_REG_HPP

#include <onejit/node.hpp>

#include <iosfwd>

namespace onejit {

class RegId {
  friend class Func;
  friend class Reg;

public:
  constexpr RegId() : val_() {
  }

  constexpr uint32_t val() const {
    return val_[0] | uint32_t(val_[1]) << 8 | uint32_t(val_[2]) << 16;
  }

private:
  constexpr explicit RegId(uint32_t val)
      : val_{uint8_t(val), uint8_t(val >> 8), uint8_t(val >> 16)} {
  }

  uint8_t val_[3];
};

constexpr bool operator==(RegId a, RegId b) {
  return a.val() == b.val();
}

constexpr bool operator!=(RegId a, RegId b) {
  return a.val() != b.val();
}

constexpr const RegId NOID = RegId();

std::ostream &operator<<(std::ostream &out, RegId id);

////////////////////////////////////////////////////////////////////////////////

class Reg {
  friend class Func;
  friend class Node;

public:
  constexpr Reg() : kind_(Void), id_() {
  }

  constexpr Type type() const {
    return REG;
  }

  constexpr Kind kind() const {
    return kind_;
  }

  constexpr uint16_t children() const {
    return 0;
  }

  constexpr RegId id() const {
    return id_;
  }

  constexpr explicit operator bool() const {
    return kind_ != Void;
  }

  constexpr operator Node() const {
    return Node(nullptr, id_.val(), NodeHeader(REG, kind_, 0));
  }

  constexpr bool is_direct() const {
    return (kind_.val() & 0x80) == 0 && (id_.val() & 0x800000) == 0;
  }
  // usable only if is_direct() returns true
  constexpr uint32_t direct() const {
    return 0x2 | uint32_t(kind_.val() & 0x7F) << 2 | id_.val() << 9;
  }
  constexpr static Reg from_direct(uint32_t data) {
    return Reg(Kind((data >> 2) & 0x7F), RegId(data >> 9));
  }

private:
  constexpr Reg(Kind kind, RegId id) : kind_(kind), id_(id) {
  }

  Kind kind_;
  RegId id_;
};

constexpr bool operator==(Reg a, Reg b) {
  return a.kind() == b.kind() && a.id() == b.id();
}

constexpr bool operator!=(Reg a, Reg b) {
  return a.kind() != b.kind() || a.id() != b.id();
}

std::ostream &operator<<(std::ostream &out, Reg reg);

} // namespace onejit

#endif // ONEJIT_REG_HPP

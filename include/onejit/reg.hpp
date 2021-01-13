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

std::ostream &operator<<(std::ostream &out, RegId regid);

////////////////////////////////////////////////////////////////////////////////

class Reg {
  friend class Func;

public:
  constexpr Reg() : kind_(Void), regid_() {
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

  constexpr operator Node() const {
    return Node(nullptr, regid_.val(), NodeHeader(REG, kind_, 0));
  }

  constexpr explicit operator bool() const {
    return kind_ != Void;
  }

  constexpr RegId regid() const {
    return regid_;
  }

private:
  constexpr Reg(Kind kind, RegId regid) : kind_(kind), regid_(regid) {
  }

  Kind kind_;
  RegId regid_;
};

constexpr bool operator==(Reg a, Reg b) {
  return a.kind() == b.kind() && a.regid() == b.regid();
}

constexpr bool operator!=(Reg a, Reg b) {
  return a.kind() != b.kind() || a.regid() != b.regid();
}

std::ostream &operator<<(std::ostream &out, Reg reg);

} // namespace onejit

#endif // ONEJIT_REG_HPP

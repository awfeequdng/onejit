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
  friend class Var;
  friend class VarExpr;

public:
  constexpr VarId() : val_() {
  }

  constexpr uint32_t val() const {
    return val_[0] | uint32_t(val_[1]) << 8 | uint32_t(val_[2]) << 16;
  }

private:
  constexpr explicit VarId(uint32_t val)
      : val_{uint8_t(val), uint8_t(val >> 8), uint8_t(val >> 16)} {
  }

  uint8_t val_[3];
};

constexpr bool operator==(VarId a, VarId b) {
  return a.val() == b.val();
}

constexpr bool operator!=(VarId a, VarId b) {
  return a.val() != b.val();
}

constexpr const VarId NOID = VarId();

std::ostream &operator<<(std::ostream &out, VarId id);

////////////////////////////////////////////////////////////////////////////////

// a local variable or register.
class Var {
  friend class Func;
  friend class Node;
  friend class VarExpr;
  friend class Test;

public:
  constexpr Var() : ekind_{kVoid}, id_{} {
  }

  constexpr Type type() const {
    return VAR;
  }

  constexpr Kind kind() const {
    return Kind{ekind_};
  }

  constexpr VarId id() const {
    return id_;
  }

  constexpr explicit operator bool() const {
    return ekind_ != kVoid;
  }

private:
  constexpr Var(Kind kind, VarId id) : ekind_{kind.val()}, id_{id} {
  }

  constexpr bool is_direct() const {
    return (ekind_ & 0x80) == 0 && (id_.val() & 0x800000) == 0 && //
           (ekind_ | id_.val()) != 0; // would be confused with type = CONTINUE
  }

  // useful only if is_direct() returns true
  constexpr uint32_t direct() const {
    return 0x2 | uint32_t(ekind_ & 0x7F) << 2 | id_.val() << 9;
  }
  static constexpr Kind parse_direct_kind(uint32_t data) {
    return Kind((data >> 2) & 0x7F);
  }
  static constexpr Var parse_direct(uint32_t data) {
    return Var{parse_direct_kind(data), VarId{data >> 9}};
  }

  // useful only if is_direct() returns false
  constexpr uint32_t indirect() const {
    return id_.val();
  }
  static constexpr Var parse_indirect(Kind kind, uint32_t data) {
    return Var{kind, VarId{data}};
  }

  eKind ekind_;
  VarId id_;
};

constexpr bool operator==(Var a, Var b) {
  return a.kind() == b.kind() && a.id() == b.id();
}

constexpr bool operator!=(Var a, Var b) {
  return a.kind() != b.kind() || a.id() != b.id();
}

std::ostream &operator<<(std::ostream &out, Var var);

} // namespace onejit

#endif // ONEJIT_VAR_HPP

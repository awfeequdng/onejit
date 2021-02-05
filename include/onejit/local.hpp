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
 * local.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_LOCAL_HPP
#define ONEJIT_LOCAL_HPP

#include <onejit/endian.hpp>
#include <onejit/node.hpp>
#include <onejit/x86/fwd.hpp>

namespace onejit {

// index of a local variable or register.
class Id {
  friend class Func;
  friend class Local;
  friend class Var;
  friend class VarHelper;
  friend class x86::Reg;

public:
  constexpr Id() noexcept : val_{} {
  }

  constexpr uint32_t val() const noexcept {
    return val_;
  }

private:
  constexpr explicit Id(uint32_t val) noexcept : val_{val & 0xFFFFFF} {
  }

  // only 24 bits are used
  uint32_t val_;
};

constexpr inline bool operator==(Id a, Id b) noexcept {
  return a.val() == b.val();
}

constexpr inline bool operator!=(Id a, Id b) noexcept {
  return a.val() != b.val();
}

constexpr const Id NOID = Id{};

const Fmt &operator<<(const Fmt &out, Id id);

////////////////////////////////////////////////////////////////////////////////

// a local variable or register.
class Local {
  friend class Code;
  friend class Func;
  friend class FuncType;
  friend class Node;
  friend class Test;
  friend class Var;
  friend class VarHelper;

public:
  // create an invalid Local.
  constexpr Local() noexcept : val_{eBad} {
  }

  constexpr Type type() const noexcept {
    return VAR;
  }

  constexpr Kind kind() const noexcept {
    return Kind{uint8_t(val_)};
  }

  constexpr Id id() const noexcept {
    return Id{val_ >> 8};
  }

  constexpr explicit operator bool() const noexcept {
    return uint8_t(val_) != eBad;
  }

protected:
  constexpr explicit Local(uint32_t val) noexcept : val_{val} {
  }

  /* create a Local for an existing Id. */
  constexpr Local(Kind kind, Id id) noexcept : val_{kind.val() | id.val() << 8} {
  }

private:
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
  static constexpr Local parse_direct(uint32_t data) noexcept {
    return Local{data >> 3};
  }

  // useful only if is_direct() returns false
  constexpr uint32_t indirect() const noexcept {
    return val_ >> 8;
  }
  static constexpr Local parse_indirect(Kind kind, uint32_t data) noexcept {
    return Local{kind.val() | data << 8};
  }

  union {
    uint32_t val_;
#ifdef ONEJIT_LITTLE_ENDIAN
    // only for debug purposes. works only on little-endian machines
    struct {
      eKind ekind_;
      uint8_t id_[3];
    };
#endif
  };
};

constexpr inline bool operator==(Local a, Local b) noexcept {
  return a.kind() == b.kind() && a.id() == b.id();
}

constexpr inline bool operator!=(Local a, Local b) noexcept {
  return a.kind() != b.kind() || a.id() != b.id();
}

const Fmt &operator<<(const Fmt &out, Local local);

} // namespace onejit

#endif // ONEJIT_LOCAL_HPP

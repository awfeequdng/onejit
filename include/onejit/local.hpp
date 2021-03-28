/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * local.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_LOCAL_HPP
#define ONEJIT_LOCAL_HPP

#include <onejit/endian.hpp>
#include <onejit/id.hpp>
#include <onejit/type.hpp>

namespace onejit {

// a local variable or register.
class Local {
  friend class Code;
  friend class Func;
  friend class Test;
  friend class ir::FuncType;
  friend class ir::Node;
  friend class ir::Var;
  friend class ir::VarHelper;

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

  const Fmt &format(const Fmt &fmt) const {
    return id().format(fmt, kind());
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

inline const Fmt &operator<<(const Fmt &fmt, Local local) {
  return local.format(fmt);
}

} // namespace onejit

#endif // ONEJIT_LOCAL_HPP

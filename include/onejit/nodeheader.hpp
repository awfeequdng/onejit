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
 * nodeheader.hpp
 *
 *  Created on Jan 12, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODEHEADER_HPP
#define ONEJIT_NODEHEADER_HPP

#include <onejit/endian.hpp>
#include <onejit/fwd.hpp>
#include <onejit/kind.hpp>
#include <onejit/opstmt.hpp>
#include <onejit/type.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// first CodeItem contained in Binary, Unary, Stmt*
union NodeHeader {

public:
  /**
   * construct an invalid NodeHeader.
   * exists only to allow placing NodeHeader in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid NodeHeader, use one of the other constructors.
   */
  constexpr NodeHeader() noexcept //
      : NodeHeader{STMT_0, Bad, BAD} {
  }

  constexpr explicit NodeHeader(CodeItem item) noexcept //
      : val_{((item >> 4) & 0x0F) | (item & ~0xFF)} {
  }

  constexpr NodeHeader(Type type, Kind kind, uint16_t op) noexcept
      : val_{(type & 0x0F) | uint32_t(kind.val()) << 8 | uint32_t(op) << 16} {
  }

  constexpr Type type() const noexcept {
    return Type(val_ & 0x0F);
  }

  constexpr Kind kind() const noexcept {
    return Kind{uint8_t(val_ >> 8)};
  }

  constexpr uint16_t op() const noexcept {
    return uint16_t(val_ >> 16);
  }

  constexpr CodeItem item() const noexcept {
    return 0xE | (val_ & 0x0F) << 4 | (val_ & ~0xFF);
  }

  constexpr explicit operator bool() const noexcept {
    return kind() != Bad;
  }

  constexpr bool operator!() const noexcept {
    return kind() == Bad;
  }

  // true if this and other NodeHeader have the same type, kind and op.
  constexpr bool operator==(const NodeHeader &other) const noexcept {
    return val_ == other.val_;
  }

  constexpr bool operator!=(const NodeHeader &other) const noexcept {
    return val_ != other.val_;
  }

private:
  CodeItem val_;
#ifdef ONEJIT_LITTLE_ENDIAN
  // only for debug purposes. works only on little-endian machines
  struct {
    Type type;
    eKind ekind;
    uint16_t op;
  } u_;
#endif
};

} // namespace onejit

#endif // ONEJIT_ARG_HPP

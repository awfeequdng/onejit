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
 *  Created on Jan 12, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODEHEADER_HPP
#define ONEJIT_NODEHEADER_HPP

#include <onejit/fwd.hpp>
#include <onejit/kind.hpp>
#include <onejit/type.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// first CodeItem contained in BinaryExpr, UnaryExpr, Stmt*
class NodeHeader {

public:
  constexpr NodeHeader() : type_{BAD}, ekind_{kBad}, op_{0} {
  }

  constexpr explicit NodeHeader(CodeItem item)
      : type_{Type((item >> 0x4) & 0xF)}, ekind_{eKind(item >> 8)}, op_{uint16_t(item >> 16)} {
  }

  constexpr NodeHeader(Type type, Kind kind, uint16_t op)
      : type_{type}, ekind_{kind.val()}, op_{op} {
  }

  constexpr Type type() const {
    return type_;
  }

  constexpr Kind kind() const {
    return Kind{ekind_};
  }

  constexpr uint16_t op() const {
    return op_;
  }

  constexpr CodeItem item() const {
    return 0x6 | uint32_t(type_ & 0xF) << 4 | uint32_t(ekind_) << 8 | uint32_t(op_) << 16;
  }

  constexpr explicit operator bool() const {
    return type_ != BAD && ekind_ != kBad;
  }

  constexpr bool operator!() const {
    return type_ == BAD || ekind_ == kBad;
  }

private:
  Type type_;
  eKind ekind_;
  uint16_t op_;
};

} // namespace onejit

#endif // ONEJIT_ARG_HPP

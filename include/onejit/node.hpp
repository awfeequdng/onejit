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
 * node.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_HPP
#define ONEJIT_NODE_HPP

#include <onejit/check.hpp>
#include <onejit/nodeheader.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// base class of BinaryExpr, ConstExpr, UnaryExpr, VarExpr, Stmt*
class Node : protected NodeHeader {
  using Base = NodeHeader;

  friend class Func;

public:
  constexpr Node() : Base(BAD), off_or_dir_{0}, code_{nullptr} {
  }

  using Base::kind;
  using Base::type;

  constexpr uint16_t children() const {
    return is_direct() || type_ == VAR || type_ == CONST
               ? 0
               : type_ == UNARY ? 1 : type_ == BINARY ? 2 : Base::op_or_children();
  }

  Node child(uint16_t i) const;

protected:
  constexpr Node(NodeHeader header, CodeItem offset_or_direct, Code *code)
      : Base{header}, off_or_dir_{offset_or_direct}, code_{code} {
  }

  constexpr NodeHeader header() const {
    return *this;
  }

  constexpr CodeItem offset_or_direct() const {
    return off_or_dir_;
  }

  constexpr Code *code() const {
    return code_;
  }

  constexpr bool is_direct() const {
    return code_ == nullptr;
  }

  // get indirect data
  CodeItem at(Offset byte_offset) const;

private:
  CodeItem off_or_dir_;
  Code *code_;
};

} // namespace onejit

#endif // ONEJIT_ARG_HPP

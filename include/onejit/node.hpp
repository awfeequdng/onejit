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
#include <onejit/code.hpp>
#include <onejit/nodeheader.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// base class of BinaryExpr, ConstExpr, UnaryExpr, VarExpr, Stmt*
class Node : protected NodeHeader {
  using Base = NodeHeader;

  friend class Const;
  friend class Var;
  friend class Stmt0;

public:
  constexpr Node() : Base(BAD), data_{0}, code_{nullptr} {
  }

  using Base::kind;
  using Base::type;

  constexpr uint16_t children() const {
    return code_ == nullptr || type_ == VAR || type_ == CONST
               ? 0
               : type_ == UNARY ? 1 : type_ == BINARY ? 2 : Base::data();
  }

  Node child(uint16_t i) const;

protected:
  constexpr Node(NodeHeader header, CodeItem offset_or_data, Code *code)
      : Base{header}, data_{offset_or_data}, code_{code} {
  }

  constexpr Code *code() const {
    return code_;
  }

  constexpr CodeItem offset_or_data() const {
    return data_;
  }

  constexpr NodeHeader header() const {
    return *this;
  }

  // get indirect data
  CodeItem at(Offset byte_offset) const {
    check(code, !=, nullptr);
    return code_->at(data_ + byte_offset);
  }

private:
  CodeItem data_;
  Code *code_;
};

} // namespace onejit

#endif // ONEJIT_ARG_HPP

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

#include <iosfwd>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// base class of BinaryExpr, ConstExpr, UnaryExpr, VarExpr, Stmt*
class Node {

  friend class Code;
  friend class ConstExpr;
  friend class VarExpr;
  friend class Func;

public:
  constexpr Node() : header_{}, off_or_dir_{0}, code_{nullptr} {
  }

  constexpr Kind kind() const {
    return header_.kind();
  }

  constexpr Type type() const {
    return header_.type();
  }

  constexpr explicit operator bool() const {
    return type() != BAD;
  }

  constexpr bool operator!() const {
    return type() == BAD;
  }

  // unified tree API: get number of children nodes
  constexpr uint16_t children() const {
    return is_direct() || type() == VAR || type() == CONST
               ? 0
               : type() == UNARY ? 1 : type() == BINARY ? 2 : header_.op_or_children();
  }

  // unified tree API: get i-th child node
  Node child(uint16_t i) const;

  // try to downcast Node to T. return T{} if fails.
  template <class T> constexpr T is() const {
    return T::is_allowed_type(type()) ? T{*this} : T{};
  }

  // try to downcast Node to T. throw exception if fails.
  template <class T> constexpr T to() const {
    return CHECK(T::is_allowed_type(type()), ==, true), T{*this};
  }

protected:
  constexpr Node(NodeHeader header, CodeItem offset_or_direct, Code *code)
      : header_{header}, off_or_dir_{offset_or_direct}, code_{code} {
  }

  constexpr NodeHeader header() const {
    return header_;
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
  NodeHeader header_;
  CodeItem off_or_dir_;
  Code *code_;
};

std::ostream &operator<<(std::ostream &out, const Node &node);

} // namespace onejit

#endif // ONEJIT_ARG_HPP

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
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_HPP
#define ONEJIT_NODE_HPP

#include <onejit/check.hpp>
#include <onejit/math.hpp>
#include <onejit/nodeheader.hpp>

#include <iosfwd>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// base class of BinaryExpr, ConstExpr, UnaryExpr, VarExpr, Stmt*
class Node {

  friend class BinaryExpr;
  friend class CallExpr;
  friend class Code;
  friend class CodeParser;
  friend class ConstExpr;
  friend class Func;
  friend class FuncType;
  friend class Label;
  friend class MemExpr;
  friend class ReturnStmt;
  friend class Stmt1;
  friend class Stmt2;
  friend class Stmt3;
  friend class Stmt4;
  friend class StmtN;
  friend class SwitchStmt;
  friend class AssignStmt;
  friend class UnaryExpr;
  friend class VarExpr;

public:
  constexpr Node() noexcept : header_{}, off_or_dir_{0}, code_{nullptr} {
  }

  constexpr Kind kind() const noexcept {
    return header_.kind();
  }

  constexpr Type type() const noexcept {
    return header_.type();
  }

  constexpr uint16_t op() const noexcept {
    return header_.op();
  }

  constexpr NodeHeader header() const noexcept {
    return header_;
  }

  constexpr explicit operator bool() const noexcept {
    return bool(header_);
  }

  constexpr bool operator!() const noexcept {
    return !header_;
  }

  // identity test: true if this and other are the same Node.
  // does not recurse on children.
  constexpr bool operator==(const Node &other) const noexcept {
    return header_ == other.header_ && off_or_dir_ == other.off_or_dir_ && code_ == other.code_;
  }

  constexpr bool operator!=(const Node &other) const noexcept {
    return !(*this == other);
  }

  // return Node length, in bytes
  Offset length() const noexcept {
    return mul_uint32(size(), sizeof(CodeItem));
  }

  // return Node length, in CodeItems
  Offset size() const noexcept;

  // unified tree API: get number of children nodes
  uint32_t children() const noexcept;

  // unified tree API: get i-th child node.
  // return Node{} if i is out of bounds
  Node child(uint32_t i) const noexcept;

  // try to downcast Node to T. return T{} if fails.
  template <class T> constexpr T is() const noexcept {
    return T::is_allowed_type(type()) && T::is_allowed_op(op()) ? T{*this} : T{};
  }

  // try to downcast Node to T. throw exception if fails.
  template <class T> constexpr T to() const {
    return ONEJIT_CHECK(T::is_allowed_type(type()), &&, T::is_allowed_op(op())), T{*this};
  }

  // return true if all children between start ... end-1 can be downcasted to T.
  template <class T> bool children_are(uint32_t start, uint32_t end) const noexcept {
    for (size_t i = start; i < end; i++) {
      if (!child(i).is<T>()) {
        return false;
      }
    }
    return true;
  }

  Node compile(Compiler &comp) const noexcept;

protected:
  constexpr Node(NodeHeader header, CodeItem offset_or_direct, const Code *code) noexcept
      : header_{header}, off_or_dir_{offset_or_direct}, code_{code} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type /*t*/) noexcept {
    return true;
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t /*op*/) noexcept {
    return true;
  }

  constexpr CodeItem offset_or_direct() const noexcept {
    return off_or_dir_;
  }

  constexpr const Code *code() const noexcept {
    return code_;
  }

  constexpr bool is_direct() const noexcept {
    return code_ == nullptr;
  }

  // get CodeItem indirect data
  // return 0 if byte_offset is out of bounds
  CodeItem get(Offset byte_offset) const noexcept {
    return uint32(byte_offset);
  }

  // get uint32_t indirect data
  // return 0 if byte_offset is out of bounds
  uint32_t uint32(Offset byte_offset) const noexcept;

  // get uint64_t indirect data
  // return 0 if byte_offset is out of bounds
  uint64_t uint64(Offset byte_offset) const noexcept;

private:
  NodeHeader header_;
  CodeItem off_or_dir_;
  const Code *code_;
}; // namespace onejit

std::ostream &operator<<(std::ostream &out, const Node &node);

} // namespace onejit

#endif // ONEJIT_ARG_HPP

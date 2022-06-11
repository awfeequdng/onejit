/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * node.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_NODE_HPP
#define ONEJIT_IR_NODE_HPP

#include <onejit/fmt.hpp>
#include <onejit/ir/allow.hpp>
#include <onejit/ir/header.hpp>
#include <onejit/ir/syntax.hpp>
#include <onejit/math.hpp>
#include <onejit/test.hpp>

#include <type_traits> // std::is_base_of<>

namespace std {
template <class T> struct hash;
}

namespace onejit {
namespace ir {

////////////////////////////////////////////////////////////////////////////////
// base class of all nodes: Binary, Const, FuncType, Label, Name, Unary, Var, Stmt*
class Node {

  friend class AssignCall;
  friend class Binary;
  friend class Call;
  friend class Const;
  friend class FuncType;
  friend class Label;
  friend class Mem;
  friend class Name;
  friend class Return;
  friend class Stmt0;
  friend class Stmt1;
  friend class Stmt2;
  friend class Stmt3;
  friend class Stmt4;
  friend class StmtN;
  friend class Switch;
  friend class Tuple;
  friend class Unary;
  friend class Var;
  friend class ::onejit::Code;
  friend class ::onejit::CodeParser;
  friend class ::onejit::Func;
  friend class ::onejit::Optimizer;

  template <class T> friend struct ::std::hash;

public:
  constexpr Node() noexcept : header_{}, off_or_dir_{0}, code_{nullptr} {
  }

  // rule-of-5: declare copy constructor because we declare assignment operator below
  Node(Node &&other) noexcept = default;
  Node(const Node &other) noexcept = default;

  constexpr Kind kind() const noexcept {
    return header_.kind();
  }

  constexpr Type type() const noexcept {
    return header_.type();
  }

  constexpr uint16_t op() const noexcept {
    return header_.op();
  }

  constexpr Header header() const noexcept {
    return header_;
  }

  Node &operator=(Node &&other) &noexcept = default;
  Node &operator=(const Node &other) &noexcept = default;

  // forbid assignment to temporary Node: we want temporary_node = expr to fail at compile time.
  // Needed because Array<Node>::operator[] returns a temporary Node by value, not a Node&
  Node &operator=(Node &&other) &&noexcept = delete;
  Node &operator=(const Node &other) &&noexcept = delete;

  /// @return true if this Node is valid
  constexpr explicit operator bool() const noexcept {
    return bool(header_);
  }

  /// @return true if this Node is invalid
  constexpr bool operator!() const noexcept {
    return !header_;
  }

  // identity test: true if this and other are the same Node.
  // does not recurse on children.
  constexpr bool operator==(const Node &other) const noexcept {
    return header_ == other.header_ && off_or_dir_ == other.off_or_dir_ && code_ == other.code_;
  }

  // identity test: true if this and other are *not* the same Node.
  // does not recurse on children.
  constexpr bool operator!=(const Node &other) const noexcept {
    return !(*this == other);
  }

  // deep comparison: return true if this and other Node are equal trees.
  // Recurses on children.
  bool deep_equal(const Node &other, Allow allow_mask = AllowAll) noexcept;

  // deep comparison:
  /// @return -1 if this tree is "less" than other tree,
  /// @return +1 if this tree is "greater" than other tree,
  // otherwise return 0. Recurses on children.
  int deep_compare(const Node &other) const noexcept;

  // true if node and its children have no side effects:
  // no memory access, no function calls, no assignments
  // i.e. only arithmetic on constants and variables.
  // Recurses on children.
  bool deep_pure(Allow allow_mask = AllowAll) const noexcept;

  /// @return Node length, in bytes
  Offset length_bytes() const noexcept {
    return mul_uint32(length_items(), sizeof(CodeItem));
  }

  /// @return Node length, in CodeItems
  Offset length_items() const noexcept;

  // unified tree API: get number of children nodes
  uint32_t children() const noexcept;

  // unified tree API: get i-th child node.
  /// @return Node{} if i is out of bounds
  Node child(uint32_t i) const noexcept;

  // try to downcast child(i) to T. return T{} if fails.
  // equivalent to child(i).is<T>()
  template <class T> constexpr ONEJIT_NOINLINE T child_is(uint32_t i) const noexcept {
    return child(i).is<T>();
  }

  // try to downcast this Node to T. return T{} if fails.
  template <class T> constexpr T is() const noexcept {
    static_assert(std::is_base_of<Node, T>::value, "Node::is(): type T must be a subclass of Node");
    return T::is_allowed_type(type()) && T::is_allowed_op(op()) ? T{*this} : T{};
  }

  // try to downcast Node to T. throw exception if fails.
  template <class T> constexpr T to() const {
    static_assert(std::is_base_of<Node, T>::value, "Node::to(): type T must be a subclass of Node");
    return ONEJIT_CHECK(T::is_allowed_type(type()), &&, T::is_allowed_op(op())), T{*this};
  }

  /// @return true if all children between start ... end-1 can be downcasted to T.
  template <class T> bool children_are(uint32_t start, uint32_t end) const noexcept {
    for (size_t i = start; i < end; i++) {
      if (!child_is<T>(i)) {
        return false;
      }
    }
    return true;
  }

  const Fmt &format(const Fmt &fmt, Syntax syntax = Syntax::Default, size_t depth = 0) const;

protected:
  constexpr Node(Header header, CodeItem offset_or_direct, const Code *code) noexcept
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
  /// @return 0 if byte_offset is out of bounds
  CodeItem get(Offset byte_offset) const noexcept {
    return uint32(byte_offset);
  }

  // get uint32_t indirect data
  /// @return 0 if byte_offset is out of bounds
  uint32_t uint32(Offset byte_offset) const noexcept;

  // get uint64_t indirect data
  /// @return 0 if byte_offset is out of bounds
  uint64_t uint64(Offset byte_offset) const noexcept;

private:
  /// @return true if result of i-th child is used.
  /// @return false if child is only evaluated for its side effects.
  bool child_result_is_used(uint32_t i) const noexcept;

  // used by Optimizer and by subclasses' create() method
  static Node create_indirect(Func &func, Header header, Nodes children) noexcept;

  // used by subclasses' create() method
  static Node create_indirect(Func &func, Header header,
                              std::initializer_list<Node> children) noexcept {
    return create_indirect(func, header, Nodes{children.begin(), children.size()});
  }

  // used by subclasses' create() method
  static Node create_indirect_from_ranges(Func &func, Header header, ChildRanges children) noexcept;

  Header header_;
  CodeItem off_or_dir_;
  const Code *code_;
};

String to_string(Node node, Syntax syntax = Syntax::Default, size_t depth = 0);

inline const Fmt &operator<<(const Fmt &fmt, const Node &node) {
  return node.format(fmt);
}

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_NODE_HPP

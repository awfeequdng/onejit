/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * node.cpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/binary.hpp>
#include <onejit/ir/childrange.hpp>
#include <onejit/ir/const.hpp>
#include <onejit/ir/functype.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/mem.hpp>
#include <onejit/ir/name.hpp>
#include <onejit/ir/node.hpp>
#include <onejit/ir/stmt0.hpp>
#include <onejit/ir/stmt1.hpp>
#include <onejit/ir/stmt2.hpp>
#include <onejit/ir/stmt3.hpp>
#include <onejit/ir/stmt4.hpp>
#include <onejit/ir/stmtn.hpp>
#include <onejit/ir/tuple.hpp>
#include <onejit/ir/unary.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/test.hpp>
#include <onestl/chars.hpp>

namespace onejit {
namespace ir {

ONEJIT_NOINLINE uint32_t Node::uint32(Offset byte_offset) const noexcept {
  return code_ ? code_->uint32(off_or_dir_ + byte_offset) : 0;
}

ONEJIT_NOINLINE uint64_t Node::uint64(Offset byte_offset) const noexcept {
  return code_ ? code_->uint64(off_or_dir_ + byte_offset) : 0;
}

ONEJIT_NOINLINE uint32_t Node::children() const noexcept {
  return is_list(type()) ? get(sizeof(CodeItem)) : to_children(type());
}

Node Node::child(uint32_t i) const noexcept {
  if (i >= children()) {
    return Node{};
  }
  // skip Header and child count
  const CodeItem item = get(sizeof(CodeItem) * (size_t(i) + (is_list(type()) ? 2 : 1)));

  Header header;
  uint32_t offset_or_direct = 0;
  const Code *code = nullptr;

  // item low bits can be:
  // 0b0000 => empty Node
  // 0b***1 => direct CONST
  // 0b**00 => indirect Node. item is relative offset
  // 0b*010 => direct VAR
  // 0b0110 => direct Stmt0
  // 0b1110 => Header

  if ((item & 1) != 0) {
    // direct Const
    offset_or_direct = item;
    header = Header{CONST, Imm::parse_direct_kind(item), 0};
  } else if ((item & 7) == 2) {
    // direct Local
    offset_or_direct = item;
    header = Header{VAR, Local::parse_direct_kind(item), 0};
  } else if ((item & 0xF) == 6) {
    // direct Stmt0
    offset_or_direct = item;
    header = Header{STMT_0, Void, Stmt0::parse_direct_op(item)};
  } else if (item && (item & 3) == 0) {
    // indirect Node: item is relative offset between parent and child
    offset_or_direct = off_or_dir_ + item;
    header = Header{code_->get(offset_or_direct)};
    code = code_; // only indirect Nodes need code
  } else {
    // Header 0b0000 is empty Node i.e. type = STMT_0, kind = eBad, op = OpStmt0::BAD, direct = 0
    //     and is the only "false" node i.e. with operator bool() == false.
    // The direct Stmt0 BadStmt{} is quite similar, it has
    //     type = STMT_0, kind = eVoid, op = OpStmt0::BAD, direct = 6.
    // TODO: Merge them?
    //
    // Header or tag 0b1110: should not appear here,
    // => return an empty node
    return Node{};
  }
  return Node{header, offset_or_direct, code};
}

Offset Node::length_items() const noexcept {
  Offset len = add_uint32(1, children());
  Offset plus = 0;
  switch (type()) {
  case VAR:
    plus = 1; // for Id
    break;
  case CONST:
    plus = (kind().bits().val() + 31) / 32;
    break;
  case LABEL:
    plus = 2; // for uint64_t address
    break;
  case NAME:
    plus = (op() + 3) / 4; // for char[] array
    break;
  default:
    if (is_list(type())) {
      // first CodeItem after header is #children
      plus = 1;
    }
    break;
  }
  return add_uint32(len, plus);
}

Node Node::create_indirect(Func &func, Header header, Nodes children) noexcept {
  Code *holder = func.code();
  const size_t n = children.size();
  while (holder && n == uint32_t(n)) {
    CodeItem offset = holder->length();

    if (holder->add(header)) {
      if (!is_list(header.type()) || holder->add_uint32(n)) {
        if (holder->add(children, offset)) {
          return Node{header, offset, holder};
        }
      }
      holder->truncate(offset);
      break;
    }
  }
  return Node{};
}

Node Node::create_indirect_from_ranges(Func &func, Header header, ChildRanges children) noexcept {
  Code *holder = func.code();
  const size_t ni = children.size();
  while (holder) {
    size_t n = 0;
    const bool islist = is_list(header.type());
    if (islist) {
      for (size_t i = 0; i < ni; i++) {
        n += children[i].size();
      }
    }
    CodeItem offset = holder->length();
    if (n == uint32_t(n) && holder->add(header)) {
      if (!islist || holder->add_uint32(n)) {
        if (holder->add_ranges(children, offset)) {
          return Node{header, offset, holder};
        }
      }
      holder->truncate(offset);
      break;
    }
  }
  return Node{};
}

constexpr inline bool is_allowed(Type t, uint16_t op, Allow allow_mask) noexcept {
  return ((allow_mask & AllowDivision) || t != BINARY || Op2(op) < QUO || Op2(op) > REM) &&
         ((allow_mask & AllowMemAccess) || t != MEM) &&
         ((allow_mask & AllowCall) || t != TUPLE || OpN(op) != CALL);
}

bool Node::deep_equal(const Node &other, Allow allow_mask) noexcept {
  if (is_direct() != other.is_direct() || header() != other.header() ||
      !is_allowed(type(), op(), allow_mask)) {
    return false;
  }
  if (is_direct()) {
    // direct nodes have no children. compare only their direct data
    return offset_or_direct() == other.offset_or_direct();
  }
  if (code() == other.code() && offset_or_direct() == other.offset_or_direct()) {
    // nodes are identical, i.e. the same node
    return true;
  }
  const size_t n = children();
  if (n != other.children()) {
    return false;
  }
  for (size_t i = 0; i < n; i++) {
    if (!child(i).deep_equal(other.child(i), allow_mask)) {
      return false;
    }
  }
  return true;
}

static constexpr int compare(size_t a, size_t b) noexcept {
  return a < b ? -1 : a > b ? 1 : 0;
}

int Node::deep_compare(const Node &other) const noexcept {
  if (header() != other.header()) {
    return header() < other.header() ? -1 : 1;
  }
  if (is_direct() != other.is_direct()) {
    // direct nodes are "less" than indirect nodes
    return is_direct() ? -1 : 1;
  }
  if (is_direct()) {
    // direct nodes have no children. compare only their direct data
    return compare(offset_or_direct(), other.offset_or_direct());
  }
  if (code() == other.code() && offset_or_direct() == other.offset_or_direct()) {
    // nodes are identical, i.e. the same node
    return 0;
  }
  const size_t n1 = children();
  const size_t n2 = other.children();
  const size_t n = n1 < n2 ? n1 : n2;
  for (size_t i = 0; i < n; i++) {
    if (int cmp = child(i).deep_compare(other.child(i))) {
      return cmp;
    }
  }
  // children up to min(n1,n2) are equal.
  // last comparison: nodes with fewer children are "less"
  return compare(n1, n2);
}

bool Node::deep_pure(Allow allow_mask) const noexcept {
  Node node = *this;
  for (;;) {
    Type t = node.type();
    if (t == VAR || t >= LABEL) {
      return true;
    } else if (t <= STMT_N || !is_allowed(t, node.op(), allow_mask)) {
      // statements exist for their side effects
      return false;
    } else if (const size_t n = node.children()) {
      // is_allowed() above already checked whether the type and operation are allowed
      for (size_t i = 0; i + 1 < n; i++) {
        if (!node.child(i).deep_pure(allow_mask)) {
          return false;
        }
      }
      node = node.child(n - 1);
      continue;
    } else {
      return true; // no children
    }
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////

const Fmt &Node::format(const Fmt &fmt, Syntax syntax, const size_t depth) const {
  const Type t = type();
  switch (t) {
  case STMT_0:
    return is<Stmt0>().format(fmt, syntax, depth);
  case STMT_1:
    return is<Stmt1>().format(fmt, syntax, depth);
  case STMT_2:
    return is<Stmt2>().format(fmt, syntax, depth);
  case STMT_3:
    return is<Stmt3>().format(fmt, syntax, depth);
  case STMT_4:
    return is<Stmt4>().format(fmt, syntax, depth);
  case STMT_N:
    return is<StmtN>().format(fmt, syntax, depth);
  case VAR:
    return is<Var>().format(fmt, syntax, depth);
  case MEM:
    return is<Mem>().format(fmt, syntax, depth);
  case UNARY:
    return is<Unary>().format(fmt, syntax, depth);
  case BINARY:
    return is<Binary>().format(fmt, syntax, depth);
  case TUPLE:
    return is<Tuple>().format(fmt, syntax, depth);
  case LABEL:
    return is<Label>().format(fmt, syntax, depth);
  case CONST:
    return is<Const>().format(fmt, syntax, depth);
  case FTYPE:
    return is<FuncType>().format(fmt, syntax, depth);
  case NAME:
    return is<Name>().format(fmt, syntax, depth);
  default:
    return fmt << to_string(t);
  }
}

String to_string(Node node, Syntax syntax, size_t depth) {
  String str;
  node.format(Fmt{&str}, syntax, depth);
  return str;
}

} // namespace ir
} // namespace onejit

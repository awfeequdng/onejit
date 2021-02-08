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
 * node.cpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/binary.hpp>
#include <onejit/check.hpp>
#include <onejit/code.hpp>
#include <onejit/const.hpp>
#include <onejit/func.hpp>
#include <onejit/functype.hpp>
#include <onejit/label.hpp>
#include <onejit/mem.hpp>
#include <onejit/name.hpp>
#include <onejit/node.hpp>
#include <onejit/stmt0.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/stmt2.hpp>
#include <onejit/stmt3.hpp>
#include <onejit/stmt4.hpp>
#include <onejit/stmtn.hpp>
#include <onejit/tuple.hpp>
#include <onejit/unary.hpp>
#include <onejit/var.hpp>
#include <onestl/chars.hpp>

namespace onejit {

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
  // skip NodeHeader and child count
  const CodeItem item = get(sizeof(CodeItem) * (size_t(i) + (is_list(type()) ? 2 : 1)));

  NodeHeader header;
  uint32_t offset_or_direct = 0;
  const Code *code = nullptr;

  // item low bits can be:
  // 0b***1 => direct CONST
  // 0b**00 => relative offset of indirect Node
  // 0b*010 => direct VAR
  // 0b0110 => unused
  // 0b1110 => NodeHeader

  if (item == 0) {
    // nothing to do
  } else if (item < 4) {
    // special case: Stmt0 is always direct,
    // only four values exist: BadStmt (handled above) Break Continue Fallthrough
    header = NodeHeader{STMT_0, Void, uint16_t(item)};
  } else if ((item & 1) != 0) {
    // direct Imm
    offset_or_direct = item;
    header = NodeHeader{CONST, Imm::parse_direct_kind(item), 0};
  } else if ((item & 7) == 2) {
    // direct Local
    offset_or_direct = item;
    header = NodeHeader{VAR, Local::parse_direct_kind(item), 0};
#if 0 // unused
  } else if ((item & 0xF) == 0xE) {
    offset_or_direct = item;
    header = NodeHeader{???, ???::parse_direct_kind(item), 0};
#endif
  } else if ((item & 3) == 0) {
    // indirect Node: item is relative offset between parent and child
    offset_or_direct = off_or_dir_ + item;
    header = NodeHeader{code_->get(offset_or_direct)};
    code = code_; // only indirect Nodes need code
  } else {
    // NodeHeader or tag 0b0110: should not appear here,
    // => return an invalid node
  }
  return Node{header, offset_or_direct, code};
}

Offset Node::length_items() const noexcept {
  Offset len = sum_uint32(1, children());
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
  return sum_uint32(len, plus);
}

Node Node::create_indirect(Func &func, NodeHeader header, Nodes children) noexcept {
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

const Fmt &Node::format(const Fmt &out, const size_t depth) const {
  const Type t = type();
  switch (t) {
  case STMT_0:
    return is<Stmt0>().format(out, depth);
  case STMT_1:
    return is<Stmt1>().format(out, depth);
  case STMT_2:
    return is<Stmt2>().format(out, depth);
  case STMT_3:
    return is<Stmt3>().format(out, depth);
  case STMT_4:
    return is<Stmt4>().format(out, depth);
  case STMT_N:
    return is<StmtN>().format(out, depth);
  case VAR:
    return is<Var>().format(out, depth);
  case MEM:
    return is<Mem>().format(out, depth);
  case UNARY:
    return is<Unary>().format(out, depth);
  case BINARY:
    return is<Binary>().format(out, depth);
  case TUPLE:
    return is<Tuple>().format(out, depth);
  case LABEL:
    return is<Label>().format(out, depth);
  case CONST:
    return is<Const>().format(out, depth);
  case FTYPE:
    return is<FuncType>().format(out, depth);
  case NAME:
    return is<Name>().format(out, depth);
  default:
    return out << to_string(t);
  }
}

} // namespace onejit

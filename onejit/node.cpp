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

#include <onejit/binaryexpr.hpp>
#include <onejit/check.hpp>
#include <onejit/code.hpp>
#include <onejit/compiler.hpp>
#include <onejit/constexpr.hpp>
#include <onejit/functype.hpp>
#include <onejit/label.hpp>
#include <onejit/memexpr.hpp>
#include <onejit/node.hpp>
#include <onejit/stmt0.hpp>
#include <onejit/stmt1.hpp>
#include <onejit/stmt2.hpp>
#include <onejit/stmt3.hpp>
#include <onejit/stmt4.hpp>
#include <onejit/stmtn.hpp>
#include <onejit/tupleexpr.hpp>
#include <onejit/unaryexpr.hpp>
#include <onejit/varexpr.hpp>
#include <onestl/chars.hpp>

namespace onejit {

uint32_t ONEJIT_NOINLINE Node::uint32(Offset byte_offset) const noexcept {
  return code_ ? code_->uint32(off_or_dir_ + byte_offset) : 0;
}

uint64_t ONEJIT_NOINLINE Node::uint64(Offset byte_offset) const noexcept {
  return code_ ? code_->uint64(off_or_dir_ + byte_offset) : 0;
}

uint32_t ONEJIT_NOINLINE Node::children() const noexcept {
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

  if (item < 4) {
    // special case: Stmt0 is always direct,
    // only four values exist: BadStmt BreakStmt ContinueStmt FallthroughStmt
    header = NodeHeader{STMT_0, Void, uint16_t(item)};
  } else if ((item & 1) != 0) {
    // direct Const
    offset_or_direct = item;
    header = NodeHeader{CONST, Const::parse_direct_kind(item), 0};
  } else if ((item & 7) == 2) {
    // direct Var
    offset_or_direct = item;
    header = NodeHeader{VAR, Var::parse_direct_kind(item), 0};
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

Offset Node::size() const noexcept {
  Offset len = sum_uint32(1, children());
  switch (type()) {
  case VAR:
    len = sum_uint32(len, 1); // for VarId
    break;
  case CONST:
    len = sum_uint32(len, (kind().bits().val() + 31) / 32);
    break;
  case LABEL:
    len = sum_uint32(len, 2); // for uint64_t address
    break;
  default:
    if (is_list(type())) {
      // first CodeItem after header is #children
      len = sum_uint32(len, 1);
    }
    break;
  }
  return len;
}

Compiler &Node::compile(Compiler &comp) const noexcept {
  const Type t = type();
  switch (t) {
  case STMT_0:
    return is<Stmt0>().compile(comp);
  case STMT_1:
    return is<Stmt1>().compile(comp);
  case STMT_2:
    return is<Stmt2>().compile(comp);
  case STMT_3:
    return is<Stmt3>().compile(comp);
  case STMT_4:
    return is<Stmt4>().compile(comp);
  case STMT_N:
    return is<StmtN>().compile(comp);
  case TUPLE:
    if (const CallExpr call = is<CallExpr>()) {
      return call.compile(comp);
    }
    // FALLTHROUGH
  default:
    return comp.add(*this);
  }
}

std::ostream &operator<<(std::ostream &out, const Node &node) {
  const Type t = node.type();
  switch (t) {
  case STMT_0:
    return out << node.is<Stmt0>();
  case STMT_1:
    return out << node.is<Stmt1>();
  case STMT_2:
    return out << node.is<Stmt2>();
  case STMT_3:
    return out << node.is<Stmt3>();
  case STMT_4:
    return out << node.is<Stmt4>();
  case STMT_N:
    return out << node.is<StmtN>();
  case VAR:
    return out << node.is<VarExpr>();
  case MEM:
    return out << node.is<MemExpr>();
  case UNARY:
    return out << node.is<UnaryExpr>();
  case BINARY:
    return out << node.is<BinaryExpr>();
  case TUPLE:
    return out << node.is<TupleExpr>();
  case LABEL:
    return out << node.is<Label>();
  case CONST:
    return out << node.is<ConstExpr>();
  case FTYPE:
    return out << node.is<FuncType>();
  default:
    return out << to_string(t);
  }
}

} // namespace onejit

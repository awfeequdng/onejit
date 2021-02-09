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
 * fwd.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FWD_HPP
#define ONEJIT_FWD_HPP

#include <onestl/fwd.hpp>

#include <cstdint> // uint*_t

#define ONEJIT_NOINLINE ONESTL_NOINLINE
#define ONEJIT_NORETURN ONESTL_NORETURN

#define ONEJIT_N_OF(array) (sizeof(array) / sizeof(array[0]))

namespace onejit {

template <class T> using Buffer = ::onestl::Buffer<T>;
using Bytes = ::onestl::Bytes;
using Chars = ::onestl::Chars;
using Fmt = ::onestl::Fmt;
using Hex = ::onestl::Hex;
template <class T> using Span = ::onestl::Span<T>;
using String = ::onestl::String;
template <class T> using Vector = ::onestl::Vector<T>;
template <class T> using View = ::onestl::View<T>;
using Writer = ::onestl::Writer;

class ArchId;
class Assembler;
class Assign;
class AssignCall;
class Bits;
class Block;
class Binary;
class Break;
class Compiler;
class Call;
class Case;
class Code;
class CodeParser;
class Comma;
class Cond;
class Const;
union Float32Bits;
union Float64Bits;
class Imm;
class Interpreter;
class Continue;
class Default;
enum class eArchId : uint8_t;
enum eBits : uint8_t;
enum eKind : uint8_t;
class Error;
class Expr;
class Fallthrough;
class For;
class Func;
class FuncType;
class Goto;
enum Group : uint8_t;
class If;
class Id;
class Local;
class JumpIf;
class Label;
class Kind;
class Mem;
class Name;
class Node;
class NodeHeader;
enum Op1 : uint16_t;
enum Op2 : uint16_t;
enum OpN : uint16_t;
enum OpStmt0 : uint16_t;
enum OpStmt1 : uint16_t;
enum OpStmt2 : uint16_t;
enum OpStmt3 : uint16_t;
enum OpStmt4 : uint16_t;
enum OpStmtN : uint16_t;
class Optimizer;
class Return;
class Stmt0;
class Stmt1;
class Stmt2;
class Stmt3;
class Stmt4;
class StmtN;
class Switch;
class Test;
class Tuple;
class Unary;
class Value;
class Var;
class VarHelper;

using CodeItem = uint32_t;
using Offset = uint32_t;
using SimdN = Bits;

using Cases = View<Case>;
using CodeItems = View<CodeItem>;
using Errors = View<Error>;
using Exprs = View<Expr>;
using Kinds = View<Kind>;
using Nodes = View<Node>;

} // namespace onejit

#endif // ONEJIT_FWD_HPP

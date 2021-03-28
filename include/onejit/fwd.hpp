/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * fwd.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FWD_HPP
#define ONEJIT_FWD_HPP

#include <onejit/ir/fwd.hpp>
#include <onestl/fwd.hpp>

#include <cstdint> // uint*_t

namespace onejit {

using namespace ::onejit::ir;

using BitSet = ::onestl::BitSet;
using Bytes = ::onestl::Bytes;
using Chars = ::onestl::Chars;
using Fmt = ::onestl::Fmt;
using Graph = ::onestl::Graph;
using Hex = ::onestl::Hex;
using String = ::onestl::String;
using Writer = ::onestl::Writer;

template <class T> using Array = ::onestl::Array<T>;
template <class T> using Buffer = ::onestl::Buffer<T>;
template <class T> using CRange = ::onestl::CRange<T>;
template <class T> using Range = ::onestl::Range<T>;
template <class T> using Span = ::onestl::Span<T>;
template <class T> using SSpan = ::onestl::SSpan<T>;
// template <class T> using View = ::onestl::View<T>; // already in onejit::ir

enum Abi : uint32_t;
enum ArchId : uint8_t;
class Assembler;
class Bits;
class BasicBlock;
enum Check : uint8_t;
class Code;
class CodeParser;
class Compiler;
union Float32Bits;
union Float64Bits;
class FlowGraph;
enum eBits : uint8_t;
enum eKind : uint8_t;
class Error;
class Func;
enum Group : uint8_t;
class Id;
class Imm;
class Kind;
class Local;
enum Op1 : uint16_t;
enum Op2 : uint16_t;
enum OpN : uint16_t;
enum OpStmt0 : uint16_t;
enum OpStmt1 : uint16_t;
enum OpStmt2 : uint16_t;
enum OpStmt3 : uint16_t;
enum OpStmt4 : uint16_t;
enum OpStmtN : uint16_t;
enum Opt : uint16_t;
class Optimizer;
class Test;
class Value;

using CodeItem = uint32_t;
using Offset = uint32_t;
using SimdN = Bits;

using BasicBlocks = View<BasicBlock>;
using CodeItems = View<CodeItem>;
using Errors = View<Error>;
using Kinds = View<Kind>;
using Values = View<Value>;

} // namespace onejit

#endif // ONEJIT_FWD_HPP

/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_expr.cpp
 *
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onejit/ir.hpp>

namespace onejit {

void Test::arch() {
  static const Chars archstring[] = {"NOARCH", "MIR", "X64", "X86", "ARM64", "ARM"};
  for (ArchId archid = ArchId(0); archid < ARCHID_N; archid = ArchId(archid + 1)) {
    TEST(to_string(archid), ==, archstring[archid]);
  }
}

void Test::kind() {
  for (uint8_t i = 0; i <= eArchFlags; i++) {
    Kind k{i};
    TEST(bool(k), ==, i != 0);
    TEST(k.is(k.group()), ==, true);
    TEST(k.simdn().val(), ==, 1);
    TEST(k.nosimd(), ==, k);
    TEST(k.bits().val(), ==, k.nosimd().bits().val());
  }
  for (uint8_t i = 0; i <= eArchFlags; i++) {
    Kind k{eKind(i), SimdN{2}};
    TEST(bool(k), ==, true);
    TEST(k.is(k.group()), ==, true);
    TEST(k.simdn().val(), ==, 2);
    TEST((Kind{k.nosimd().val(), k.simdn()}), ==, k);
    TEST(k.bits().val(), ==, k.nosimd().bits().val() * k.simdn().val());
  }
  // dump_and_clear_code();
}

// test that integer Imm can be compiled as 'constexpr'
extern constexpr const Imm one_million{uint64_t(1000000ul)};
extern constexpr const Imm one_billion{uint64_t(1000000000ul)};

void Test::const_expr() const {
  TEST(Const{Void}, ==, VoidConst);
  TEST(Const{true}, ==, TrueConst);
  TEST(Const{false}, ==, FalseConst);

  Const one{Int8, int16_t(1)};
  Const minus_one{Int8, int16_t(-1)};
  TEST(one.imm(), ==, Imm{int8_t(1)});
  TEST(minus_one.imm(), ==, Imm{int8_t(-1)});

  Const plus_10k{Int16, int16_t(10000)};
  Const minus_10k{Int16, int16_t(-10000)};
  TEST(plus_10k.imm(), ==, Imm{int16_t(10000)});
  TEST(minus_10k.imm(), ==, Imm{int16_t(-10000)});

  Const plus_32k{Int16, int16_t(32767)};
  Const minus_32k{Int16, int16_t(-32768)};
  TEST(plus_32k.imm(), ==, Imm{int16_t(32767)});
  TEST(minus_32k.imm(), ==, Imm{int16_t(-32768)});
}

void Test::simple_expr() {
  Imm imm{1.5f};
  Expr c = Const{func, imm};

  TEST(imm.kind(), ==, Float32);
  TEST(imm.float32(), ==, 1.5f);
  TEST(c.is<Const>().imm(), ==, imm);

  TEST(c.type(), ==, CONST);
  TEST(c.kind(), ==, Float32);
  TEST(c.op(), ==, 0);
  TEST(c.children(), ==, 0);
  TEST(c.is<Node>(), ==, c);
  TEST(c.is<Expr>(), ==, c);
  TEST(c.is<Binary>(), ==, Binary{});
  TEST(c.is<Call>(), ==, Call{});
  TEST(c.is<Const>(), ==, c);
  TEST(c.is<Mem>(), ==, Mem{});
  TEST(c.is<Stmt>(), ==, Stmt{});
  TEST(c.is<Tuple>(), ==, Tuple{});
  TEST(c.is<Unary>(), ==, Unary{});
  TEST(c.is<Var>(), ==, Var{});
  TEST(bool(c), ==, true);
  TEST(bool(c.is<Binary>()), ==, false);
  TEST(bool(c.is<Call>()), ==, false);
  TEST(bool(c.is<Const>()), ==, true);
  TEST(bool(c.is<Expr>()), ==, true);
  TEST(bool(c.is<Mem>()), ==, false);
  TEST(bool(c.is<Node>()), ==, true);
  TEST(bool(c.is<Stmt>()), ==, false);
  TEST(bool(c.is<Unary>()), ==, false);
  TEST(bool(c.is<Var>()), ==, false);
  TEST(bool(c.is<Tuple>()), ==, false);

  for (uint8_t i = eVoid; i <= eArchFlags; i++) {
    Kind k = Kind(i);
    Expr v = Var{func, k};
    Node node = Tuple{func, ADD, v, c};

    TEST(v.type(), ==, VAR);
    TEST(v.kind(), ==, k);
    TEST(v.op(), ==, 0);
    TEST(v.children(), ==, 0);
    TEST(v.is<Node>(), ==, v);
    TEST(v.is<Expr>(), ==, v);
    TEST(v.is<Binary>(), ==, Binary{});
    TEST(v.is<Call>(), ==, Call{});
    TEST(v.is<Const>(), ==, Const{});
    TEST(v.is<Mem>(), ==, Mem{});
    TEST(v.is<Stmt>(), ==, Stmt{});
    TEST(v.is<Tuple>(), ==, Tuple{});
    TEST(v.is<Unary>(), ==, Unary{});
    TEST(v.is<Var>(), ==, v);
    TEST(bool(v), ==, true);
    TEST(bool(v.is<Binary>()), ==, false);
    TEST(bool(v.is<Expr>()), ==, true);
    TEST(bool(v.is<Call>()), ==, false);
    TEST(bool(v.is<Const>()), ==, false);
    TEST(bool(v.is<Mem>()), ==, false);
    TEST(bool(v.is<Node>()), ==, true);
    TEST(bool(v.is<Stmt>()), ==, false);
    TEST(bool(v.is<Unary>()), ==, false);
    TEST(bool(v.is<Var>()), ==, true);
    TEST(bool(v.is<Tuple>()), ==, false);

    TEST(node.type(), ==, TUPLE);
    TEST(node.kind(), ==, k);
    TEST(node.op(), ==, ADD);
    TEST(node.children(), ==, 2);
    TEST(node.child(0), ==, v);
    TEST(node.child(1), ==, c);
    TEST(node.is<Node>(), ==, node);
    TEST(node.is<Expr>(), ==, node);
    TEST(node.is<Binary>(), ==, Binary{});
    TEST(node.is<Const>(), ==, Const{});
    TEST(node.is<Tuple>(), ==, node);
    TEST(node.is<Stmt>(), ==, Stmt{});
    TEST(bool(node), ==, true);
    TEST(bool(node.is<Node>()), ==, true);
    TEST(bool(node.is<Expr>()), ==, true);
    TEST(bool(node.is<Binary>()), ==, false);
    TEST(bool(node.is<Const>()), ==, false);
    TEST(bool(node.is<Stmt>()), ==, false);
    TEST(bool(node.is<Tuple>()), ==, true);

    Local local1 = v.to<Var>().local();
    Local local2 = Local::parse_direct(local1.direct());
    TEST(local1, ==, local2);
    TEST(local1.kind(), ==, k);
  }
  // dump_and_clear_code();
  holder.clear();
}

void Test::nested_expr() {
  for (uint8_t i = eInt8; i <= eUint64; i++) {
    Kind k{i};

    Expr c1 = Const{func, Imm{k, 1}};
    Expr c2 = Const{func, Imm{k, 2}};

    Expr v1 = Var{func, k};
    Expr v2 = Var{func, k};

    Expr b1 = Tuple{func, ADD, c1, v1};
    Expr b2 = Tuple{func, MUL, c2, v2};
    Expr b3 = Binary{func, SHL, b1, b2};
    Expr u1 = Unary{func, XOR1, b3};

    TEST(b1.kind(), ==, k);
    TEST(b2.kind(), ==, k);
    TEST(b3.kind(), ==, k);
    TEST(u1.kind(), ==, k);

    TEST(b1.child(0), ==, c1);
    TEST(b1.child(1), ==, v1);
    TEST(b2.child(0), ==, c2);
    TEST(b2.child(1), ==, v2);
    TEST(b3.child(0), ==, b1);
    TEST(b3.child(1), ==, b2);
    TEST(u1.child(0), ==, b3);
  }
  // dump_and_clear_code();
  holder.clear();
}

} // namespace onejit

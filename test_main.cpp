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
 * test_main.cpp
 *
 *  Created on Jan 08, 2020
 *      Author Massimiliano Ghilardi
 */

#include <onejit/codeparser.hpp>
#include <onejit/func.hpp>
#include <onestl/chars.hpp>

#include <fstream>
#include <iostream>

namespace onejit {
class Test {
public:
  Test();
  ~Test();

  void run();
  void kind();
  void simple_expr();
  void nested_expr();
  void dump_code() const;

private:
  Code holder;
  Func func;
};

Test::Test() : holder{}, func{&holder} {
}

Test::~Test() {
}

void Test::run() {
  kind();
  simple_expr();
  nested_expr();
  dump_code();
}

void Test::kind() {
  for (uint8_t i = 0; i <= kArchFlags; i++) {
    const Kind k{i};
    ONEJIT_TEST(bool(k), ==, i != 0);
    ONEJIT_TEST(k.is(k.group()), ==, true);
    ONEJIT_TEST(k.simdn().val(), ==, 1);
    ONEJIT_TEST(k.nosimd(), ==, k);
    ONEJIT_TEST(k.bits().val(), ==, k.nosimd().bits().val());

    std::cout << "Kind " << k << ", Group " << k.group() << ", bits " << k.bits() << '\n';
  }
  for (uint8_t i = 0; i <= kArchFlags; i++) {
    const Kind k{eKind(i), SimdN{2}};
    ONEJIT_TEST(bool(k), ==, true);
    ONEJIT_TEST(k.is(k.group()), ==, true);
    ONEJIT_TEST(k.simdn().val(), ==, 2);
    ONEJIT_TEST((Kind{k.nosimd().val(), k.simdn()}), ==, k);
    ONEJIT_TEST(k.bits().val(), ==, k.nosimd().bits().val() * k.simdn().val());
  }
}

void Test::simple_expr() {
  const Const c{1.5f};
  const Expr ce = func.new_const(c);

  ONEJIT_TEST(c.kind(), ==, Float32);
  ONEJIT_TEST(c.float32(), ==, 1.5f);
  ONEJIT_TEST(ce.is<ConstExpr>().constant(), ==, c);

  ONEJIT_TEST(ce.type(), ==, CONST);
  ONEJIT_TEST(ce.kind(), ==, Float32);
  ONEJIT_TEST(ce.op(), ==, 0);
  ONEJIT_TEST(ce.children(), ==, 0);
  ONEJIT_TEST(ce.is<Node>(), ==, ce);
  ONEJIT_TEST(ce.is<Expr>(), ==, ce);
  ONEJIT_TEST(ce.is<BinaryExpr>(), ==, BinaryExpr{});
  ONEJIT_TEST(ce.is<ConstExpr>(), ==, ce);
  ONEJIT_TEST(ce.is<UnaryExpr>(), ==, UnaryExpr{});
  ONEJIT_TEST(ce.is<VarExpr>(), ==, VarExpr{});
  ONEJIT_TEST(ce.is<Stmt>(), ==, Stmt{});
  ONEJIT_TEST(bool(ce), ==, true);
  ONEJIT_TEST(bool(ce.is<Node>()), ==, true);
  ONEJIT_TEST(bool(ce.is<Expr>()), ==, true);
  ONEJIT_TEST(bool(ce.is<BinaryExpr>()), ==, false);
  ONEJIT_TEST(bool(ce.is<ConstExpr>()), ==, true);
  ONEJIT_TEST(bool(ce.is<UnaryExpr>()), ==, false);
  ONEJIT_TEST(bool(ce.is<VarExpr>()), ==, false);
  ONEJIT_TEST(bool(ce.is<Stmt>()), ==, false);

  for (uint8_t i = kVoid; i <= kArchFlags; i++) {
    const Kind k = Kind(i);
    const Expr ve = func.new_var(k);
    const Node node = func.new_binary(k, ADD, ce, ve);

    std::cout << node.type() << ' ' << node << '\n';
    ONEJIT_TEST(ve.type(), ==, VAR);
    ONEJIT_TEST(ve.kind(), ==, k);
    ONEJIT_TEST(ve.op(), ==, 0);
    ONEJIT_TEST(ve.children(), ==, 0);
    ONEJIT_TEST(ve.is<Node>(), ==, ve);
    ONEJIT_TEST(ve.is<Expr>(), ==, ve);
    ONEJIT_TEST(ve.is<BinaryExpr>(), ==, BinaryExpr{});
    ONEJIT_TEST(ve.is<ConstExpr>(), ==, ConstExpr{});
    ONEJIT_TEST(ve.is<UnaryExpr>(), ==, UnaryExpr{});
    ONEJIT_TEST(ve.is<VarExpr>(), ==, ve);
    ONEJIT_TEST(ve.is<Stmt>(), ==, Stmt{});
    ONEJIT_TEST(bool(ve), ==, true);
    ONEJIT_TEST(bool(ve.is<Node>()), ==, true);
    ONEJIT_TEST(bool(ve.is<Expr>()), ==, true);
    ONEJIT_TEST(bool(ve.is<BinaryExpr>()), ==, false);
    ONEJIT_TEST(bool(ve.is<ConstExpr>()), ==, false);
    ONEJIT_TEST(bool(ve.is<UnaryExpr>()), ==, false);
    ONEJIT_TEST(bool(ve.is<VarExpr>()), ==, true);
    ONEJIT_TEST(bool(ve.is<Stmt>()), ==, false);

    ONEJIT_TEST(node.type(), ==, BINARY);
    ONEJIT_TEST(node.kind(), ==, k);
    ONEJIT_TEST(node.op(), ==, ADD);
    ONEJIT_TEST(node.children(), ==, 2);
    ONEJIT_TEST(node.child(0), ==, ce);
    ONEJIT_TEST(node.child(1), ==, ve);
    ONEJIT_TEST(node.is<Node>(), ==, node);
    ONEJIT_TEST(node.is<Expr>(), ==, node);
    ONEJIT_TEST(node.is<BinaryExpr>(), ==, node);
    ONEJIT_TEST(node.is<ConstExpr>(), ==, ConstExpr{});
    ONEJIT_TEST(node.is<Stmt>(), ==, Stmt{});
    ONEJIT_TEST(bool(node), ==, true);
    ONEJIT_TEST(bool(node.is<Node>()), ==, true);
    ONEJIT_TEST(bool(node.is<Expr>()), ==, true);
    ONEJIT_TEST(bool(node.is<BinaryExpr>()), ==, true);
    ONEJIT_TEST(bool(node.is<ConstExpr>()), ==, false);
    ONEJIT_TEST(bool(node.is<Stmt>()), ==, false);

    const Var var1 = ve.to<VarExpr>().var();
    const Var var2 = Var::parse_direct(var1.direct());
    ONEJIT_TEST(var1, ==, var2);
    ONEJIT_TEST(var1.kind(), ==, k);
  }
}

void Test::nested_expr() {
  for (uint8_t i = kInt8; i <= kInt64; i++) {
    const Kind k{i};

    const Const c1{k, 1};
    const Const c2{k, 2};
    const Expr ce1 = func.new_const(c1);
    const Expr ce2 = func.new_const(c2);

    const Expr ve1 = func.new_var(k);
    const Expr ve2 = func.new_var(k);

    const Expr be1 = func.new_binary(k, ADD, ce1, ve1);
    const Expr be2 = func.new_binary(k, MUL, ce2, ve2);
    const Expr be3 = func.new_binary(k, SHL, be1, be2);
    const Expr ue4 = func.new_unary(k, XOR1, be3);

    ONEJIT_TEST(be1.child(0), ==, ce1);
    ONEJIT_TEST(be1.child(1), ==, ve1);
    ONEJIT_TEST(be2.child(0), ==, ce2);
    ONEJIT_TEST(be2.child(1), ==, ve2);
    ONEJIT_TEST(be3.child(0), ==, be1);
    // ONEJIT_TEST(be3.child(1), ==, be2); // currently broken

    std::cout << ue4.type() << ' ' << ue4 << '\n';
  }
}

void Test::dump_code() const {
  std::cout << std::hex;
  for (const CodeItem item : holder) {
    std::cout << Chars("0x") << item << ' ';
  }
  std::cout << '\n' << std::dec;

  std::ofstream file("dump.1jit", std::ios::out | std::ios::trunc);
  file.write(reinterpret_cast<const char *>(holder.data()), holder.length());

  CodeParser parser(&holder);
  while (parser) {
    Node node = parser.next();
    std::cout << node.type() << ' ' << node << '\n';
  }
}

} // namespace onejit

using namespace onejit;

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  onejit::Test{}.run();

  return 0;
}

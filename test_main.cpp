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
 *  Created on Jan 08, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/codeparser.hpp>
#include <onejit/compiler.hpp>
#include <onejit/func.hpp>
#include <onestl/chars.hpp>

#include <fstream>
#include <iostream>

namespace onejit {

class Test {
public:
  Test();
  ~Test();

  FuncType ftype();

  void run();
  // called by run()
  void kind();
  void const_expr() const;
  void simple_expr();
  void nested_expr();
  void fib();
  void compile(Func &func);
  void dump_code() const;

private:
  Code holder;
  Func func;
};

Test::Test() : holder{}, func{"test_func", ftype(), &holder} {
}

Test::~Test() {
}

FuncType Test::ftype() {
  const Kind params[] = {Int64, Ptr, Uint64};
  const Kind results[] = {Int64};
  FuncType ftype{Kinds{params, ONEJIT_N_OF(params)}, Kinds{results, ONEJIT_N_OF(results)}, &holder};
  std::cout << ftype << '\n';
  return ftype;
}

void Test::run() {
  kind();
  const_expr();
  simple_expr();
  nested_expr();
  fib();
  dump_code();
}

void Test::kind() {
  for (uint8_t i = 0; i <= kArchFlags; i++) {
    Kind k{i};
    ONEJIT_TEST(bool(k), ==, i != 0);
    ONEJIT_TEST(k.is(k.group()), ==, true);
    ONEJIT_TEST(k.simdn().val(), ==, 1);
    ONEJIT_TEST(k.nosimd(), ==, k);
    ONEJIT_TEST(k.bits().val(), ==, k.nosimd().bits().val());

    std::cout << "Kind " << k << ", Group " << k.group() << ", bits " << k.bits() << '\n';
  }
  for (uint8_t i = 0; i <= kArchFlags; i++) {
    Kind k{eKind(i), SimdN{2}};
    ONEJIT_TEST(bool(k), ==, true);
    ONEJIT_TEST(k.is(k.group()), ==, true);
    ONEJIT_TEST(k.simdn().val(), ==, 2);
    ONEJIT_TEST((Kind{k.nosimd().val(), k.simdn()}), ==, k);
    ONEJIT_TEST(k.bits().val(), ==, k.nosimd().bits().val() * k.simdn().val());
  }
}

// test that integer Imm can be compiled as 'constexpr'
extern constexpr const Imm one_million{uint64_t(1000000ul)};
extern constexpr const Imm one_billion{uint64_t(1000000000ul)};

void Test::const_expr() const {
  ONEJIT_TEST(Const{Void}, ==, VoidExpr);
  ONEJIT_TEST(Const{true}, ==, TrueExpr);
  ONEJIT_TEST(Const{false}, ==, FalseExpr);

  Const one{Int8, int16_t(1)};
  Const minus_one{Int8, int16_t(-1)};
  ONEJIT_TEST(one.imm(), ==, Imm{int8_t(1)});
  ONEJIT_TEST(minus_one.imm(), ==, Imm{int8_t(-1)});

  Const plus_10k{Int16, int16_t(10000)};
  Const minus_10k{Int16, int16_t(-10000)};
  ONEJIT_TEST(plus_10k.imm(), ==, Imm{int16_t(10000)});
  ONEJIT_TEST(minus_10k.imm(), ==, Imm{int16_t(-10000)});

  Const plus_32k{Int16, int16_t(32767)};
  Const minus_32k{Int16, int16_t(-32768)};
  ONEJIT_TEST(plus_32k.imm(), ==, Imm{int16_t(32767)});
  ONEJIT_TEST(minus_32k.imm(), ==, Imm{int16_t(-32768)});
}

void Test::simple_expr() {
  Imm imm{1.5f};
  Expr c = Const{&holder, imm};

  ONEJIT_TEST(imm.kind(), ==, Float32);
  ONEJIT_TEST(imm.float32(), ==, 1.5f);
  ONEJIT_TEST(c.is<Const>().imm(), ==, imm);

  ONEJIT_TEST(c.type(), ==, CONST);
  ONEJIT_TEST(c.kind(), ==, Float32);
  ONEJIT_TEST(c.op(), ==, 0);
  ONEJIT_TEST(c.children(), ==, 0);
  ONEJIT_TEST(c.is<Node>(), ==, c);
  ONEJIT_TEST(c.is<Expr>(), ==, c);
  ONEJIT_TEST(c.is<Binary>(), ==, Binary{});
  ONEJIT_TEST(c.is<Const>(), ==, c);
  ONEJIT_TEST(c.is<Unary>(), ==, Unary{});
  ONEJIT_TEST(c.is<Var>(), ==, Var{});
  ONEJIT_TEST(c.is<Stmt>(), ==, Stmt{});
  ONEJIT_TEST(bool(c), ==, true);
  ONEJIT_TEST(bool(c.is<Node>()), ==, true);
  ONEJIT_TEST(bool(c.is<Expr>()), ==, true);
  ONEJIT_TEST(bool(c.is<Binary>()), ==, false);
  ONEJIT_TEST(bool(c.is<Const>()), ==, true);
  ONEJIT_TEST(bool(c.is<Unary>()), ==, false);
  ONEJIT_TEST(bool(c.is<Var>()), ==, false);
  ONEJIT_TEST(bool(c.is<Stmt>()), ==, false);

  for (uint8_t i = kVoid; i <= kArchFlags; i++) {
    Kind k = Kind(i);
    Expr v = Var{func, k};
    Node node = func.new_binary(ADD, v, c);

    ONEJIT_TEST(v.type(), ==, VAR);
    ONEJIT_TEST(v.kind(), ==, k);
    ONEJIT_TEST(v.op(), ==, 0);
    ONEJIT_TEST(v.children(), ==, 0);
    ONEJIT_TEST(v.is<Node>(), ==, v);
    ONEJIT_TEST(v.is<Expr>(), ==, v);
    ONEJIT_TEST(v.is<Binary>(), ==, Binary{});
    ONEJIT_TEST(v.is<Const>(), ==, Const{});
    ONEJIT_TEST(v.is<Unary>(), ==, Unary{});
    ONEJIT_TEST(v.is<Var>(), ==, v);
    ONEJIT_TEST(v.is<Stmt>(), ==, Stmt{});
    ONEJIT_TEST(bool(v), ==, true);
    ONEJIT_TEST(bool(v.is<Node>()), ==, true);
    ONEJIT_TEST(bool(v.is<Expr>()), ==, true);
    ONEJIT_TEST(bool(v.is<Binary>()), ==, false);
    ONEJIT_TEST(bool(v.is<Const>()), ==, false);
    ONEJIT_TEST(bool(v.is<Unary>()), ==, false);
    ONEJIT_TEST(bool(v.is<Var>()), ==, true);
    ONEJIT_TEST(bool(v.is<Stmt>()), ==, false);

    ONEJIT_TEST(node.type(), ==, BINARY);
    ONEJIT_TEST(node.kind(), ==, k);
    ONEJIT_TEST(node.op(), ==, ADD);
    ONEJIT_TEST(node.children(), ==, 2);
    ONEJIT_TEST(node.child(0), ==, v);
    ONEJIT_TEST(node.child(1), ==, c);
    ONEJIT_TEST(node.is<Node>(), ==, node);
    ONEJIT_TEST(node.is<Expr>(), ==, node);
    ONEJIT_TEST(node.is<Binary>(), ==, node);
    ONEJIT_TEST(node.is<Const>(), ==, Const{});
    ONEJIT_TEST(node.is<Stmt>(), ==, Stmt{});
    ONEJIT_TEST(bool(node), ==, true);
    ONEJIT_TEST(bool(node.is<Node>()), ==, true);
    ONEJIT_TEST(bool(node.is<Expr>()), ==, true);
    ONEJIT_TEST(bool(node.is<Binary>()), ==, true);
    ONEJIT_TEST(bool(node.is<Const>()), ==, false);
    ONEJIT_TEST(bool(node.is<Stmt>()), ==, false);

    Local local1 = v.to<Var>().local();
    Local local2 = Local::parse_direct(local1.direct());
    ONEJIT_TEST(local1, ==, local2);
    ONEJIT_TEST(local1.kind(), ==, k);
  }
}

void Test::nested_expr() {
  for (uint8_t i = kInt8; i <= kUint64; i++) {
    Kind k{i};

    Expr c1 = Const{&holder, Imm{k, 1}};
    Expr c2 = Const{&holder, Imm{k, 2}};

    Expr v1 = Var{func, k};
    Expr v2 = Var{func, k};

    Expr b1 = func.new_binary(ADD, c1, v1);
    Expr b2 = func.new_binary(MUL, c2, v2);
    Expr b3 = func.new_binary(SHL, b1, b2);
    Expr u1 = func.new_unary(XOR1, b3);

    ONEJIT_TEST(b1.kind(), ==, k);
    ONEJIT_TEST(b2.kind(), ==, k);
    ONEJIT_TEST(b3.kind(), ==, k);
    ONEJIT_TEST(u1.kind(), ==, k);

    ONEJIT_TEST(b1.child(0), ==, c1);
    ONEJIT_TEST(b1.child(1), ==, v1);
    ONEJIT_TEST(b2.child(0), ==, c2);
    ONEJIT_TEST(b2.child(1), ==, v2);
    ONEJIT_TEST(b3.child(0), ==, b1);
    ONEJIT_TEST(b3.child(1), ==, b2);
    ONEJIT_TEST(u1.child(0), ==, b3);
  }
}

void Test::fib() {
  const Kind kind = Uint64;
  const Kinds kinds{&kind, 1};
  Func f("fib", FuncType{kinds, kinds, &holder}, &holder);
  Var param = f.param(0);
  Const one = One(Uint64);
  Const two = Two(Uint64);

  f.set_body( //
      Block{&holder,
            If{&holder,                                                     //
               f.new_binary(GTR, param, two),                               //
               Return{&holder,                                              //
                      f.new_binary(                                         //
                          ADD,                                              //
                          f.new_call(f, {f.new_binary(SUB, param, one)}),   //
                          f.new_call(f, {f.new_binary(SUB, param, two)}))}, //
               Return{&holder, one}}});                                     //

  compile(f);
}

void Test::compile(Func &f) {
  Compiler{f}.compile().finish();
}

void Test::dump_code() const {
  std::cout << std::hex;
  for (CodeItem item : holder) {
    std::cout << Chars("0x") << item << ' ';
  }
  std::cout << '\n' << std::dec;

  std::ofstream file("dump.1jit", std::ios::out | std::ios::trunc);
  file.write(reinterpret_cast<const char *>(holder.data()), holder.length());

  CodeParser parser(&holder);
  while (parser) {
    Node node = parser.next();
    std::cout << node << '\n';
  }
}

} // namespace onejit

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  onejit::Test{}.run();

  return 0;
}

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

// test that integer Const can be compiled as 'constexpr'
extern constexpr const Const one_million{uint64_t(1000000ul)};
extern constexpr const Const one_billion{uint64_t(1000000000ul)};

void Test::const_expr() const {
  ONEJIT_TEST(ConstExpr{Void}, ==, VoidExpr);
  ONEJIT_TEST(ConstExpr{true}, ==, TrueExpr);
  ONEJIT_TEST(ConstExpr{false}, ==, FalseExpr);

  ConstExpr one{Int8, int16_t(1)};
  ConstExpr minus_one{Int8, int16_t(-1)};
  ONEJIT_TEST(one.constant(), ==, Const{int8_t(1)});
  ONEJIT_TEST(minus_one.constant(), ==, Const{int8_t(-1)});

  ConstExpr plus_10k{Int16, int16_t(10000)};
  ConstExpr minus_10k{Int16, int16_t(-10000)};
  ONEJIT_TEST(plus_10k.constant(), ==, Const{int16_t(10000)});
  ONEJIT_TEST(minus_10k.constant(), ==, Const{int16_t(-10000)});

  ConstExpr plus_32k{Int16, int16_t(32767)};
  ConstExpr minus_32k{Int16, int16_t(-32768)};
  ONEJIT_TEST(plus_32k.constant(), ==, Const{int16_t(32767)});
  ONEJIT_TEST(minus_32k.constant(), ==, Const{int16_t(-32768)});
}

void Test::simple_expr() {
  Const c{1.5f};
  Expr ce = func.new_const(c);

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
    Kind k = Kind(i);
    Expr ve = func.new_var(k);
    Node node = func.new_binary(ADD, ve, ce);

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
    ONEJIT_TEST(node.child(0), ==, ve);
    ONEJIT_TEST(node.child(1), ==, ce);
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

    Var var1 = ve.to<VarExpr>().var();
    Var var2 = Var::parse_direct(var1.direct());
    ONEJIT_TEST(var1, ==, var2);
    ONEJIT_TEST(var1.kind(), ==, k);
  }
}

void Test::nested_expr() {
  for (uint8_t i = kInt8; i <= kUint64; i++) {
    Kind k{i};

    Const c1{k, 1};
    Const c2{k, 2};
    Expr ce1 = func.new_const(c1);
    Expr ce2 = func.new_const(c2);

    Expr ve1 = func.new_var(k);
    Expr ve2 = func.new_var(k);

    Expr be1 = func.new_binary(ADD, ce1, ve1);
    Expr be2 = func.new_binary(MUL, ce2, ve2);
    Expr be3 = func.new_binary(SHL, be1, be2);
    Expr ue1 = func.new_unary(XOR1, be3);

    ONEJIT_TEST(be1.kind(), ==, k);
    ONEJIT_TEST(be2.kind(), ==, k);
    ONEJIT_TEST(be3.kind(), ==, k);
    ONEJIT_TEST(ue1.kind(), ==, k);

    ONEJIT_TEST(be1.child(0), ==, ce1);
    ONEJIT_TEST(be1.child(1), ==, ve1);
    ONEJIT_TEST(be2.child(0), ==, ce2);
    ONEJIT_TEST(be2.child(1), ==, ve2);
    ONEJIT_TEST(be3.child(0), ==, be1);
    ONEJIT_TEST(be3.child(1), ==, be2);
    ONEJIT_TEST(ue1.child(0), ==, be3);
  }
}

void Test::fib() {
  const Kind kind = Uint64;
  const Kinds kinds{&kind, 1};
  Func f("fib", FuncType{kinds, kinds, &holder}, &holder);
  VarExpr param = f.param(0);
  ConstExpr one = One(Uint64);
  ConstExpr two = Two(Uint64);

  f.set_body(                                                           //
      f.new_block(                                                      //
          f.new_if(                                                     //
              f.new_binary(GTR, param, two),                            //
              f.new_return(                                             //
                  f.new_binary(                                         //
                      ADD,                                              //
                      f.new_call(f, {f.new_binary(SUB, param, one)}),   //
                      f.new_call(f, {f.new_binary(SUB, param, two)}))), //
              f.new_return(one))));                                     //

  compile(f);
}

void Test::compile(Func &f) {
  Compiler comp(f);
  f.get_body().compile(comp);

  comp.finish();
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

using namespace onejit;

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  onejit::Test{}.run();

  return 0;
}

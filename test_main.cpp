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

#include <onejit/onejit.hpp>

#include <cstdio> // stdout

#ifdef __unix__
#include <fcntl.h>     // open()
#include <sys/stat.h>  //
#include <sys/types.h> //
#include <unistd.h>    // close(), write()
#endif

namespace onejit {

class Test {
public:
  Test();
  ~Test();

  void run();

private:
  FuncType ftype();
  static String to_string(Node node);
  void dump_and_clear_code();

  // called by run()
  void kind();
  void const_expr() const;
  void simple_expr();
  void nested_expr();
  void func_fib();
  void func_loop();
  void func_switch1();
  void func_switch2();
  void func_cond();
  void compile(Func &func);

  Code holder;
  Func func;
  Compiler comp;
};

Test::Test() : holder{}, func{&holder, Name{&holder, "test_func"}, ftype()} {
}

Test::~Test() {
}

FuncType Test::ftype() {
  FuncType ftype{&holder, {Int64, Ptr, Uint64}, {Int64}};
  Fmt{stdout} << ftype << '\n';
  return ftype;
}

String Test::to_string(Node node) {
  return onestl::to_string(node);
}

void Test::run() {
  kind();
  const_expr();
  simple_expr();
  nested_expr();
  func_fib();
  func_loop();
  func_switch1();
  func_switch2();
  // func_cond();
}

void Test::kind() {
  Fmt out{stdout};
  for (uint8_t i = 0; i <= kArchFlags; i++) {
    Kind k{i};
    ONEJIT_TEST(bool(k), ==, i != 0);
    ONEJIT_TEST(k.is(k.group()), ==, true);
    ONEJIT_TEST(k.simdn().val(), ==, 1);
    ONEJIT_TEST(k.nosimd(), ==, k);
    ONEJIT_TEST(k.bits().val(), ==, k.nosimd().bits().val());

    out << "Kind " << k << ", Group " << k.group() << ", bits " << k.bits() << '\n';
  }
  for (uint8_t i = 0; i <= kArchFlags; i++) {
    Kind k{eKind(i), SimdN{2}};
    ONEJIT_TEST(bool(k), ==, true);
    ONEJIT_TEST(k.is(k.group()), ==, true);
    ONEJIT_TEST(k.simdn().val(), ==, 2);
    ONEJIT_TEST((Kind{k.nosimd().val(), k.simdn()}), ==, k);
    ONEJIT_TEST(k.bits().val(), ==, k.nosimd().bits().val() * k.simdn().val());
  }
  dump_and_clear_code();
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
  Expr c = Const{func, imm};

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
    Node node = Binary{func, ADD, v, c};

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
  // dump_and_clear_code();
  holder.clear();
}

void Test::nested_expr() {
  for (uint8_t i = kInt8; i <= kUint64; i++) {
    Kind k{i};

    Expr c1 = Const{func, Imm{k, 1}};
    Expr c2 = Const{func, Imm{k, 2}};

    Expr v1 = Var{func, k};
    Expr v2 = Var{func, k};

    Expr b1 = Binary{func, ADD, c1, v1};
    Expr b2 = Binary{func, MUL, c2, v2};
    Expr b3 = Binary{func, SHL, b1, b2};
    Expr u1 = Unary{func, XOR1, b3};

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
  // dump_and_clear_code();
  holder.clear();
}

void Test::func_fib() {
  Kind kind = Uint64;
  Func &f = func.reset(&holder, Name{&holder, "fib"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Const one = One(kind);
  Const two = Two(kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t fib(uint64_n) {
   *   if (n > 2) {
   *     return f(n-1) + f(n-2);
   *   } else {
   *     return 1;
   *   }
   * }
   */

  f.set_body(                                                  //
      If{f, Binary{f, GTR, n, two},                            //
         Return{f,                                             //
                Binary{f,                                      //
                       ADD,                                    //
                       Call{f, f, {Binary{f, SUB, n, one}}},   //
                       Call{f, f, {Binary{f, SUB, n, two}}}}}, //
         Return{f, one}});                                     //

  Chars expected = "(if (> var1000_ul 2_ul)\n\
    (return (+ (call label_0 (- var1000_ul 1_ul)) (call label_0 (- var1000_ul 2_ul))))\n\
    (return 1_ul))";
  ONEJIT_TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (jump_if label_1 (! (> var1000_ul 2_ul)))\n\
    (= var1002_ul (- var1000_ul 1_ul))\n\
    (= var1003_ul (call label_0 var1002_ul))\n\
    (= var1004_ul (- var1000_ul 2_ul))\n\
    (= var1005_ul (call label_0 var1004_ul))\n\
    (= var1001_ul (+ var1003_ul var1005_ul))\n\
    (return var1001_ul)\n\
    (goto label_2)\n\
    label_1\n\
    (= var1001_ul 1_ul)\n\
    (return var1001_ul)\n\
    label_2)";
  ONEJIT_TEST(to_string(f.get_compiled()), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_loop() {
  Kind kind = Uint64;
  Func &f = func.reset(&holder, Name{&holder, "loop"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Var total = f.result(0);
  Var i{f, kind};
  Const zero = Zero(kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t loop(uint64_n) {
   *   uint64_t total = 0, i;
   *   for (i = 0; i < n; i++) {
   *     total += i;
   *   }
   *   return total;
   * }
   */

  f.set_body( //
      Block{f,
            {Assign{f, ASSIGN, total, zero},
             For{
                 f,                              //
                 Assign{f, ASSIGN, i, zero},     // init
                 Binary{f, LSS, i, n},           // test
                 Inc{f, i},                      // post
                 Assign{f, ADD_ASSIGN, total, i} // body
             },
             Return{f, total}}});

  Chars expected = "(block\n\
    (= var1001_ul 0_ul)\n\
    (for (= var1002_ul 0_ul) (< var1002_ul var1000_ul) (++ var1002_ul)\n\
    (+= var1001_ul var1002_ul))\n\
    (return var1001_ul))";
  ONEJIT_TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (= var1001_ul 0_ul)\n\
    (= var1002_ul 0_ul)\n\
    (goto label_2)\n\
    label_1\n\
    (+= var1001_ul var1002_ul)\n\
    (++ var1002_ul)\n\
    label_2\n\
    (jump_if label_1 (< var1002_ul var1000_ul))\n\
    label_3\n\
    (return var1001_ul))";
  ONEJIT_TEST(to_string(f.get_compiled()), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_switch1() {
  Kind kind = Uint64;
  Func &f = func.reset(&holder, Name{&holder, "fswitch1"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Var ret = f.result(0);
  Const zero = Zero(kind);
  Const one = One(kind);
  Const two = Two(kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t fswitch1(uint64_n) {
   *   uint64_t ret;
   *   switch (n) {
   *     case 0:
   *       ret = 1;
   *       break;
   *     case 1:
   *       ret = 2;
   *       break;
   *     default:
   *       ret = n + 1;
   *       break;
   *   }
   *   return ret;
   * }
   */

  f.set_body( //
      Block{f,
            {Switch{f,
                    n,
                    {Case{f, zero, Assign{f, ASSIGN, ret, one}}, //
                     Case{f, one, Assign{f, ASSIGN, ret, two}},  //
                     Default{f, Assign{f, ASSIGN, ret, Binary{f, ADD, n, one}}}}},
             Return{f, ret}}});

  Chars expected = "(block\n\
    (switch\n\
    var1000_ul\n\
    (case 0_ul (= var1001_ul 1_ul))\n\
    (case 1_ul (= var1001_ul 2_ul))\n\
    (default (= var1001_ul (+ var1000_ul 1_ul))))\n\
    (return var1001_ul))";
  ONEJIT_TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (jump_if label_2 (!= var1000_ul 0_ul))\n\
    (= var1001_ul 1_ul)\n\
    (goto label_1)\n\
    label_2\n\
    (jump_if label_4 (!= var1000_ul 1_ul))\n\
    label_3\n\
    (= var1001_ul 2_ul)\n\
    (goto label_1)\n\
    label_4\n\
    label_5\n\
    (= var1001_ul (+ var1000_ul 1_ul))\n\
    label_1\n\
    (return var1001_ul))";
  ONEJIT_TEST(to_string(f.get_compiled()), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_switch2() {
  Kind kind = Uint64;
  Func &f = func.reset(&holder, Name{&holder, "fswitch2"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Var ret = f.result(0);
  Const zero = Zero(kind);
  Const one = One(kind);
  Const two = Two(kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t fswitch2(uint64_n) {
   *   uint64_t ret;
   *   switch (n) {
   *     case 0:
   *       ret = 1;
   *       break;
   *     default:
   *       ret = n + 1;
   *       break;
   *     case 1:
   *       ret = 2;
   *       break;
   *   }
   *   return ret;
   * }
   */

  f.set_body( //
      Block{f,
            {Switch{f,
                    n,
                    {Case{f, zero, Assign{f, ASSIGN, ret, one}},                 //
                     Default{f, Assign{f, ASSIGN, ret, Binary{f, ADD, n, one}}}, //
                     Case{f, one, Assign{f, ASSIGN, ret, two}}}},                //
             Return{f, ret}}});

  Chars expected = "(block\n\
    (switch\n\
    var1000_ul\n\
    (case 0_ul (= var1001_ul 1_ul))\n\
    (default (= var1001_ul (+ var1000_ul 1_ul)))\n\
    (case 1_ul (= var1001_ul 2_ul)))\n\
    (return var1001_ul))";
  ONEJIT_TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (jump_if label_2 (!= var1000_ul 0_ul))\n\
    (= var1001_ul 1_ul)\n\
    (goto label_1)\n\
    label_2\n\
    (goto label_4)\n\
    label_3\n\
    (= var1001_ul (+ var1000_ul 1_ul))\n\
    (goto label_1)\n\
    label_4\n\
    (jump_if label_3 (!= var1000_ul 1_ul))\n\
    label_5\n\
    (= var1001_ul 2_ul)\n\
    label_1\n\
    (return var1001_ul))";
  ONEJIT_TEST(to_string(f.get_compiled()), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}

void Test::func_cond() {
  Kind kind = Uint64;
  Func &f = func.reset(&holder, Name{&holder, "fcond"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Var ret = f.result(0);
  Const zero = Zero(kind);
  Const one = One(kind);
  Const two = Two(kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t fswitch(uint64_n) {
   *   uint64_t ret;
   *   if (n == 0) {
   *     ret = 1;
   *   } else if (n == 1) {
   *     ret = 2;
   *   } else if (true) {
   *     ret = n + 1;
   *   }
   *   return ret;
   * }
   */

  f.set_body( //
      Block{f,
            {Cond{f,
                  {Binary{f, EQL, n, zero}, Assign{f, ASSIGN, ret, one},       //
                   Binary{f, EQL, n, one}, Assign{f, ASSIGN, ret, two},        //
                   TrueExpr, Assign{f, ASSIGN, ret, Binary{f, ADD, n, one}}}}, //
             Return{f, ret}}});

  Chars expected = "(block\n\
    (cond\n\
    (== var1000_ul 0_ul)\n\
    (= var1001_ul 1_ul)\n\
    (== var1000_ul 1_ul)\n\
    (= var1001_ul 2_ul)\n\
    true_e\n\
    (= var1001_ul (+ var1000_ul 1_ul)))\n\
    (return var1001_ul))";
  ONEJIT_TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (jump_if label_2 (! (== var1000_ul 0_ul)))\n\
    (= var1001_ul 1_ul)\n\
    (goto label_1)\n\
    label_2\n\
    (jump_if label_3 (! (== var1000_ul 1_ul)))\n\
    (= var1001_ul 2_ul)\n\
    (goto label_1)\n\
    label_3\n\
    (jump_if label_1 (! true_e))\n\
    (= var1001_ul (+ var1000_ul 1_ul))\n\
    label_1\n\
    (return var1001_ul))";
  ONEJIT_TEST(to_string(f.get_compiled()), ==, expected);

  // dump_and_clear_code();
  holder.clear();
}
void Test::compile(Func &f) {
  comp.compile(f);
}

void Test::dump_and_clear_code() {
  Fmt out{stdout};
  for (CodeItem item : holder) {
    out << "0x" << Hex{item} << ' ';
  }
  out << '\n';

#ifdef __unix__
  {
    int fd = ::open("dump.1jit", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd >= 0) {
      (void)::write(fd, holder.data(), holder.length());
      (void)::close(fd);
    }
  }
#endif // __unix__

  CodeParser parser{&holder};
  while (parser) {
    Node node = parser.next();
    out << node << '\n';
  }

  holder.clear();
}

} // namespace onejit

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  onejit::Test{}.run();

  return 0;
}

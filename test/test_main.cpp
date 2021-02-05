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

#include <onejit/assembler.hpp>
#include <onejit/onejit.hpp>
#include <onejit/x64/asm.hpp>
#include <onejit/x64/mem.hpp>
#include <onejit/x64/reg.hpp>
#include <onejit/x64/scale.hpp>

#include "test_disasm.hpp"

#include <cstdio> // stdout

#ifdef __unix__
#include <fcntl.h>     // open()
#include <sys/stat.h>  //
#include <sys/types.h> //
#include <unistd.h>    // close(), write()
#endif

namespace onejit {

class Test : public TestDisasm {
public:
  Test();
  ~Test();

  void run();

private:
  FuncType ftype();
  void dump_and_clear_code();

  // called by run()
  void kind();
  void const_expr() const;
  void simple_expr();
  void nested_expr();
  void x64_expr();
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
  FuncType functype{&holder, {Int64, Ptr, Uint64}, {Int64}};
  Chars expected = "(ftype (int64 ptr uint64) -> (int64))";
  TEST(to_string(functype), ==, expected);
  return functype;
}

void Test::run() {
  kind();
  const_expr();
  simple_expr();
  nested_expr();
  x64_expr();
  func_fib();
  func_loop();
  func_switch1();
  func_switch2();
  func_cond();

  Fmt{stdout} << testcount() << " tests passed\n";
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
  TEST(Const{Void}, ==, VoidExpr);
  TEST(Const{true}, ==, TrueExpr);
  TEST(Const{false}, ==, FalseExpr);

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
  TEST(c.is<Const>(), ==, c);
  TEST(c.is<Unary>(), ==, Unary{});
  TEST(c.is<Var>(), ==, Var{});
  TEST(c.is<Stmt>(), ==, Stmt{});
  TEST(bool(c), ==, true);
  TEST(bool(c.is<Node>()), ==, true);
  TEST(bool(c.is<Expr>()), ==, true);
  TEST(bool(c.is<Binary>()), ==, false);
  TEST(bool(c.is<Const>()), ==, true);
  TEST(bool(c.is<Unary>()), ==, false);
  TEST(bool(c.is<Var>()), ==, false);
  TEST(bool(c.is<Stmt>()), ==, false);

  for (uint8_t i = eVoid; i <= eArchFlags; i++) {
    Kind k = Kind(i);
    Expr v = Var{func, k};
    Node node = Binary{func, ADD, v, c};

    TEST(v.type(), ==, VAR);
    TEST(v.kind(), ==, k);
    TEST(v.op(), ==, 0);
    TEST(v.children(), ==, 0);
    TEST(v.is<Node>(), ==, v);
    TEST(v.is<Expr>(), ==, v);
    TEST(v.is<Binary>(), ==, Binary{});
    TEST(v.is<Const>(), ==, Const{});
    TEST(v.is<Unary>(), ==, Unary{});
    TEST(v.is<Var>(), ==, v);
    TEST(v.is<Stmt>(), ==, Stmt{});
    TEST(bool(v), ==, true);
    TEST(bool(v.is<Node>()), ==, true);
    TEST(bool(v.is<Expr>()), ==, true);
    TEST(bool(v.is<Binary>()), ==, false);
    TEST(bool(v.is<Const>()), ==, false);
    TEST(bool(v.is<Unary>()), ==, false);
    TEST(bool(v.is<Var>()), ==, true);
    TEST(bool(v.is<Stmt>()), ==, false);

    TEST(node.type(), ==, BINARY);
    TEST(node.kind(), ==, k);
    TEST(node.op(), ==, ADD);
    TEST(node.children(), ==, 2);
    TEST(node.child(0), ==, v);
    TEST(node.child(1), ==, c);
    TEST(node.is<Node>(), ==, node);
    TEST(node.is<Expr>(), ==, node);
    TEST(node.is<Binary>(), ==, node);
    TEST(node.is<Const>(), ==, Const{});
    TEST(node.is<Stmt>(), ==, Stmt{});
    TEST(bool(node), ==, true);
    TEST(bool(node.is<Node>()), ==, true);
    TEST(bool(node.is<Expr>()), ==, true);
    TEST(bool(node.is<Binary>()), ==, true);
    TEST(bool(node.is<Const>()), ==, false);
    TEST(bool(node.is<Stmt>()), ==, false);

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

    Expr b1 = Binary{func, ADD, c1, v1};
    Expr b2 = Binary{func, MUL, c2, v2};
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

void Test::x64_expr() {
  Func &f = func.reset(&holder, Name{&holder, "x64_expr"}, FuncType{&holder, {}, {}});

  {
    x64::Mem mem{f, Int32, -67890};

    Chars expected = "(x86_mem32 -67890)";
    TEST(to_string(mem), ==, expected);
  }

  Assembler assembler;

  // imm
  for (int16_t val = -0x90; val <= 0x90; val += 0x10) {
    Const c{Int32, val};
    Stmt1 st{f, c, X86_PUSH};

    test_asm_disasm_x64(st, assembler);
  }

  // reg8
  for (x64::RegId i = x64::RAX; i <= x64::R15; i = i + 1) {
    x64::Reg reg{Uint8, i};
    for (OpStmt1 op = X86_SETA; op <= X86_SETS; op = op + 1) {
      Stmt1 st{f, Var{reg}, op};

      test_asm_disasm_x64(st, assembler);
    }
  }

  // reg8, reg16, reg32, reg64
  for (Kind kind : {Uint8, Uint16, Uint32, Uint64}) {
    for (x64::RegId i = x64::RAX; i <= x64::R15; i = i + 1) {
      x64::Reg reg{kind, i};

      for (OpStmt1 op : {X86_DEC, X86_INC, X86_NEG, X86_NOT, X86_POP, X86_PUSH}) {
        if ((op == X86_POP || op == X86_PUSH) && kind != Uint16 && kind != Uint64) {
          // on x64, pop and push only support 16 bit or 64 bit argument
          continue;
        }
        Stmt1 st{f, Var{reg}, op};

        test_asm_disasm_x64(st, assembler);
      }
    }
  }

  // (mem8,16,32,64 offset base)
  for (Kind kind : {Uint8, Uint16, Uint32, Uint64}) {
    for (x64::RegId i = x64::RAX; i <= x64::R15; i = i + 1) {
      x64::Reg base{Uint64, i};
      x64::Mem mem{f, kind, 0x7f, Var{base}};

      for (OpStmt1 op : {/*X86_DEC, X86_INC, X86_NEG, X86_NOT,*/ X86_POP, X86_PUSH}) {
        if ((op == X86_POP || op == X86_PUSH) && kind != Uint16 && kind != Uint64) {
          // on x64, pop and push only support 16 bit or 64 bit argument
          continue;
        }
        Stmt1 st{f, mem, op};

        test_asm_disasm_x64(st, assembler);
      }
    }
  }

  // (mem64 offset base)
  for (x64::RegId i = x64::RAX; i <= x64::R15; i = i + 1) {
    x64::Reg base{Uint64, i};
    x64::Mem mem{f, Uint64, 0x7f, Var{base}};
    Stmt1 st{f, mem, X86_CALL};

    test_asm_disasm_x64(st, assembler);
  }

  // (mem64 offset _ index scale)
  for (x64::RegId i = x64::RAX; i <= x64::R15; i = i + 1) {
    if (i == x64::RSP) {
      // cannot encode RSP as index register
      continue;
    }
    x64::Reg index{Uint64, i};

    for (x64::Scale scale = x64::Scale1; scale <= x64::Scale8; scale <<= 1) {
      x64::Mem mem{f, Uint64, 0x7f, Var{}, Var{index}, scale};

      for (OpStmt1 op : {X86_CALL, X86_JMP}) {
        Stmt1 st{f, mem, op};

        test_asm_disasm_x64(st, assembler);
      }
    }
  }

  // (mem64 offset base index scale)
  for (x64::RegId i = x64::RAX; i <= x64::R15; i = i + 1) {
    for (x64::RegId j = x64::RAX; j <= x64::R15; j = j + 1) {
      if (j == x64::RSP) {
        // cannot encode RSP as index register
        continue;
      }
      x64::Reg base{Uint64, i}, index{Uint64, j};
      for (x64::Scale scale = x64::Scale1; scale <= x64::Scale8; scale <<= 1) {
        x64::Mem mem{f, Uint64, 0x7f /*0x77665544*/, Var{base}, Var{index}, scale};

        for (OpStmt1 op : {X86_CALL, X86_JMP}) {
          Stmt1 st{f, mem, op};

          test_asm_disasm_x64(st, assembler);
        }
      }
    }
  }

  holder.clear();
  assembler.clear();

  {
    x64::Reg base{Uint64, x64::RAX}, index{Uint64, x64::RCX};
    x64::Mem mem{f, Label{f}, 12345, Var{base}, Var{index}, x64::Scale8};
    Stmt1 st{f, mem, X86_CALL};

    assembler.x64(st);

    Chars expected = "(x86_mem64 label_1 12345 rax rcx 8)";
    TEST(to_string(mem), ==, expected);

    expected = "(x86_call (x86_mem64 label_1 12345 rax rcx 8))";
    TEST(to_string(st), ==, expected);

    TEST(assembler.size(), ==, 7);
    expected = "\xff\x94\xc8\x39\x30\x0\x0"; // call *0x3039(%rax,%rcx,8)
    TEST(assembler, ==, expected);
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

  Chars expected = "(if (> var1000_ul 2)\n\
    (return (+ (call label_0 (- var1000_ul 1)) (call label_0 (- var1000_ul 2))))\n\
    (return 1))";
  TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (jump_if label_1 (! (> var1000_ul 2)))\n\
    (= var1002_ul (- var1000_ul 1))\n\
    (= var1003_ul (call label_0 var1002_ul))\n\
    (= var1004_ul (- var1000_ul 2))\n\
    (= var1005_ul (call label_0 var1004_ul))\n\
    (= var1001_ul (+ var1003_ul var1005_ul))\n\
    (return var1001_ul)\n\
    (goto label_2)\n\
    label_1\n\
    (= var1001_ul 1)\n\
    (return var1001_ul)\n\
    label_2)";
  TEST(to_string(f.get_compiled()), ==, expected);

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
    (= var1001_ul 0)\n\
    (for (= var1002_ul 0) (< var1002_ul var1000_ul) (++ var1002_ul)\n\
    (+= var1001_ul var1002_ul))\n\
    (return var1001_ul))";
  TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (= var1001_ul 0)\n\
    (= var1002_ul 0)\n\
    (goto label_2)\n\
    label_1\n\
    (+= var1001_ul var1002_ul)\n\
    (++ var1002_ul)\n\
    label_2\n\
    (jump_if label_1 (< var1002_ul var1000_ul))\n\
    label_3\n\
    (return var1001_ul))";
  TEST(to_string(f.get_compiled()), ==, expected);

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
    (case 0 (= var1001_ul 1))\n\
    (case 1 (= var1001_ul 2))\n\
    (default (= var1001_ul (+ var1000_ul 1))))\n\
    (return var1001_ul))";
  TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (jump_if label_2 (!= var1000_ul 0))\n\
    (= var1001_ul 1)\n\
    (goto label_1)\n\
    label_2\n\
    (jump_if label_4 (!= var1000_ul 1))\n\
    label_3\n\
    (= var1001_ul 2)\n\
    (goto label_1)\n\
    label_4\n\
    label_5\n\
    (= var1001_ul (+ var1000_ul 1))\n\
    label_1\n\
    (return var1001_ul))";
  TEST(to_string(f.get_compiled()), ==, expected);

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
    (case 0 (= var1001_ul 1))\n\
    (default (= var1001_ul (+ var1000_ul 1)))\n\
    (case 1 (= var1001_ul 2)))\n\
    (return var1001_ul))";
  TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (jump_if label_2 (!= var1000_ul 0))\n\
    (= var1001_ul 1)\n\
    (goto label_1)\n\
    label_2\n\
    (goto label_4)\n\
    label_3\n\
    (= var1001_ul (+ var1000_ul 1))\n\
    (goto label_1)\n\
    label_4\n\
    (jump_if label_3 (!= var1000_ul 1))\n\
    label_5\n\
    (= var1001_ul 2)\n\
    label_1\n\
    (return var1001_ul))";
  TEST(to_string(f.get_compiled()), ==, expected);

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
    (== var1000_ul 0)\n\
    (= var1001_ul 1)\n\
    (== var1000_ul 1)\n\
    (= var1001_ul 2)\n\
    true\n\
    (= var1001_ul (+ var1000_ul 1)))\n\
    (return var1001_ul))";
  TEST(to_string(f.get_body()), ==, expected);

  compile(f);

  expected = "(block\n\
    (jump_if label_2 (! (== var1000_ul 0)))\n\
    (= var1001_ul 1)\n\
    (goto label_1)\n\
    label_2\n\
    (jump_if label_3 (! (== var1000_ul 1)))\n\
    (= var1001_ul 2)\n\
    (goto label_1)\n\
    label_3\n\
    (jump_if label_1 (! true))\n\
    (= var1001_ul (+ var1000_ul 1))\n\
    label_1\n\
    (return var1001_ul))";
  TEST(to_string(f.get_compiled()), ==, expected);

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
    int fd = ::open("dump.1jit", O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

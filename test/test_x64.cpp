/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_x64.cpp
 *
 *  Created on Feb 10, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onejit/assembler.hpp>
#include <onejit/ir.hpp>
#include <onejit/x64.hpp>

namespace onejit {

static bool is_compatible(OpStmt1 op, Kind kind) noexcept {
  if (op == X86_POP || op == X86_PUSH) {
    // pop and push only support 16 bit or 64 bit argument on x64.
    return kind == Uint16 || kind == Uint64;
  } else if (op == X86_CALL || op == X86_JMP) {
    // call and jmp always require 64 bit argument.
    return kind == Uint64;
  } else if ((op >= X86_SETA && op <= X86_SETS) || op == X86_CLFLUSH || op == X86_CLFLUSHOPT) {
    return kind == Uint8;
  } else {
    return true;
  }
}

void Test::expr_x64() {
  Func &f = func.reset(&holder, Name{&holder, "expr_x64"}, FuncType{&holder, {}, {}});

  {
    x64::Mem mem{f, Int32, x64::Address{-67890}};

    Chars expected = "(x86_mem_i -67890)";
    TEST(to_string(mem), ==, expected);
  }

  Assembler assembler;

  // imm
  for (OpStmt1 op = X86_JA; op <= X86_JMP + 1; op = op + 1) {
    if (op == X86_JMP + 1) {
      op = X86_PUSH;
    }
    for (int16_t val = -0x1000; val <= 0x1000; val += 0x37) {
      Const c{Int32, val};
      Stmt1 st{f, c, op};

      test_asm_disasm_x64(st, assembler);
    }
  }

  // reg8
  for (OpStmt1 op = X86_SETA; op <= X86_SETS; op = op + 1) {
    for (x64::RegId i = x64::RAX; i <= x64::R15; i = i + 1) {
      x64::Reg reg{Uint8, i};
      Stmt1 st{f, Var{reg}, op};
      test_asm_disasm_x64(st, assembler);
    }
  }

  // reg8,16,32,64
  for (OpStmt1 op : {X86_CALL, X86_DEC, X86_INC, X86_JMP, X86_NEG, X86_NOT, X86_POP, X86_PUSH}) {
    for (Kind kind : {Uint8, Uint16, Uint32, Uint64}) {
      if (!is_compatible(op, kind)) {
        continue;
      }
      for (x64::RegId i = x64::RAX; i <= x64::R15; i = i + 1) {
        x64::Reg reg{kind, i};

        Stmt1 st{f, Var{reg}, op};
        test_asm_disasm_x64(st, assembler);
      }
    }
  }

  // mem
  for (OpStmt1 op : {
           X86_CALL,  X86_DEC,   X86_INC,   X86_JMP,   X86_NEG,   X86_NOT,  X86_POP,   X86_PUSH, //
           X86_SETA,  X86_SETAE, X86_SETB,  X86_SETBE, X86_SETE,  X86_SETG, X86_SETGE, X86_SETL, //
           X86_SETLE, X86_SETNE, X86_SETNO, X86_SETNP, X86_SETNS, X86_SETO, X86_SETP,  X86_SETS, //
       }) {
    for (Kind kind : {Uint8, Uint16, Uint32, Uint64}) {
      if (!is_compatible(op, kind)) {
        continue;
      }

      // (mem_ub,us,ui,ul offset)
      for (int32_t offset : {0x7f, 0x77665544}) {
        x64::Mem mem{f, kind, x64::Address{offset}};

        Stmt1 st{f, mem, op};
        test_asm_disasm_x64(st, assembler);
      }

      for (x64::RegId i = x64::RAX; i <= x64::R15 + 1; i = i + 1) {
        if (i == x64::R15 + 1) {
          i = x64::RIP;
        }

        // (mem_ub,us,ui,ul offset base)
        {
          x64::Reg base{Uint64, i};
          for (int32_t offset : {0x7f, 0x77665544}) {
            x64::Mem mem{f, kind, x64::Address{offset, Var{base}}};

            Stmt1 st{f, mem, op};
            test_asm_disasm_x64(st, assembler);
          }
        }

        if (i == x64::RIP) {
          // base register = RIP cannot encode any index register
          continue;
        }

        // (mem_ub,us,ui,ul offset _ index scale)
        if (i != x64::RSP) {
          // cannot encode RSP as index register
          x64::Reg index{Uint64, i};

          for (x64::Scale scale = x64::Scale1; scale <= x64::Scale8; scale <<= 1) {
            for (int32_t offset : {0x7f, 0x77665544}) {
              x64::Mem mem{f, kind, x64::Address{offset, Var{}, Var{index}, scale}};

              Stmt1 st{f, mem, op};
              test_asm_disasm_x64(st, assembler);
            }
          }
        }

        // (mem_ub,us,ui,ul offset base index scale)
        for (x64::RegId j = x64::RAX; j <= x64::R15; j = j + 1) {
          if (j == x64::RSP) {
            // cannot encode RSP as index register
            continue;
          }
          x64::Reg base{Uint64, i}, index{Uint64, j};
          for (x64::Scale scale = x64::Scale1; scale <= x64::Scale8; scale <<= 1) {
            for (int32_t offset : {0x7f, 0x77665544}) {
              x64::Mem mem{f, kind, x64::Address{offset, Var{base}, Var{index}, scale}};

              Stmt1 st{f, mem, op};

              test_asm_disasm_x64(st, assembler);
            }
          }
        }
      }
    }
  }

  holder.clear();
  assembler.clear();

  {
    x64::Reg base{Uint64, x64::RAX}, index{Uint64, x64::RCX};
    x64::Mem mem{f, Ptr, x64::Address{Label{f}, 12345, Var{base}, Var{index}, x64::Scale8}};
    Stmt1 st{f, mem, X86_CALL};

    assembler.x64(st);

    Chars expected = "(x86_mem_p label_1 12345 rax rcx 8)";
    TEST(to_string(mem), ==, expected);

    expected = "(x86_call (x86_mem_p label_1 12345 rax rcx 8))";
    TEST(to_string(st), ==, expected);

    TEST(assembler.size(), ==, 7);
    expected = "\xff\x94\xc8\x39\x30\x0\x0"; // call *0x3039(%rax,%rcx,8)
    TEST(assembler, ==, expected);
  }

  // dump_and_clear_code();
  holder.clear();
}

} // namespace onejit

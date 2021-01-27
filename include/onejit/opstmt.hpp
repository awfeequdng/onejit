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
 * opstmt.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPSTMT_HPP
#define ONEJIT_OPSTMT_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint8_t

namespace onejit {

enum OpStmt0 : uint16_t {
  BAD = 0,
  BREAK = 1,
  CONTINUE = 2,
  FALLTHROUGH = 3,

  X86_CLC,      // clear carry flag
  X86_CLD,      // clear direction flag
  X86_CMC,      // complement carry flag
  X86_INT3,     // generate a breakpoint trap
  X86_LFENCE,   // serialize memory load operations
  X86_LOCK,     // lock prefix for following instruction
  X86_MFENCE,   // serialize all memory operations
  X86_NOP,      // no operation
  X86_PAUSE,    // spin loop hint
  X86_SFENCE,   // serialize memory store operations
  X86_SYSCALL,  // fast system call
  X86_SYSENTER, // fast system call
  X86_SYSEXIT,  // fast system call return
  X86_SYSRET,   // fast system call return
  X86_UD2,      // undefined instruction, intentionally causes SIGILL

  X86_XACQUIRE, // lock elision hint
  X86_XRELEASE, // lock elision hint

  // the following instructions require [CPUID RTM]
  X86_XEND,  // end TSX transaction
  X86_XTEST, // if TSX transaction or lock elision is in progress, clear ZF flag. otherwise set ZF
};

enum OpStmt1 : uint16_t {
  BAD_ST1 = 0,
  GOTO = 1,
  INC = 2, // ++
  DEC = 3, // --

  ASM_JA = 32, // jump if above
  ASM_JAE,     // jump if above or equal
  ASM_JB,      // jump if below
  ASM_JBE,     // jump if below or equal
  ASM_JE,      // jump if equal (if zero)
  ASM_JG,      // jump if greater
  ASM_JGE,     // jump if greater or equal
  ASM_JL,      // jump if less
  ASM_JLE,     // jump if less or equal
  ASM_JNE,     // jump if not equal (if not zero)

  X86_BSWAP = 256,    // x = byteswap(x) - invert endianity of 32bit or 64bit register
  X86_CALL,           // call function. argument is relative offset, register or memory
  X86_CBW,            // sign-extend  %al ->  %ax
  X86_CWDE = X86_CBW, // sign-extend  %ax -> %eax
  X86_CDQE = X86_CBW, // sign-extend %eax -> %rax
  X86_INT,            // generate a call to interrupt procedure. argument is immediate
  X86_JA,             // jump if above
  X86_JAE,            // jump if above or equal (if not carry)
  X86_JB,             // jump if below (if carry)
  X86_JBE,            // jump if below or equal
  X86_JE,             // jump if equal (if zero)
  X86_JG,             // jump if greater
  X86_JGE,            // jump if greater or equal
  X86_JL,             // jump if less
  X86_JLE,            // jump if less or equal
  X86_JNE,            // jump if not equal (if not zero)
  X86_JNO,            // jump if not overflow
  X86_JNP,            // jump if not parity (if odd)
  X86_JNS,            // jump if not sign
  X86_JO,             // jump if overflow
  X86_JP,             // jump if parity (if even)
  X86_JS,             // jump if sign
  X86_JMP,            // unconditional jump. argument is relative offset, register or memory
  X86_NEG,            // negate (i.e. -x) register or memory
  X86_NOT,            // invert (i.e. ^x) register or memory
  X86_POP,            // pop 1,2,4 or 8 bytes from stack into register or memory
  X86_POPF,           // pop 2 or 4 bytes from stack into EFLAGS
  X86_PUSH,           // push 1,2,4 or 8 bytes to stack from register or memory
  X86_PUSHF,          // push 2 or 4 bytes to stack from EFLAGS
  X86_RDTSC,          // read timestamp counter into %rdx:%rax
  X86_RET,            // return from function call
  X86_SETA,           // set 1 byte register or memory if above
  X86_SETAE,          // set 1 byte register or memory if above or equal (if not carry)
  X86_SETB,           // set 1 byte register or memory if below (if carry)
  X86_SETBE,          // set 1 byte register or memory if below or equal
  X86_SETE,           // set 1 byte register or memory if equal (if zero)
  X86_SETG,           // set 1 byte register or memory if greater
  X86_SETGE,          // set 1 byte register or memory if greater or equal
  X86_SETL,           // set 1 byte register or memory if less
  X86_SETLE,          // set 1 byte register or memory if less or equal
  X86_SETNE,          // set 1 byte register or memory if not equal (if not zero)
  X86_SETNO,          // set 1 byte register or memory if not overflow
  X86_SETNP,          // set 1 byte register or memory if not parity (if odd)
  X86_SETNS,          // set 1 byte register or memory if not sign
  X86_SETO,           // set 1 byte register or memory if overflow
  X86_SETP,           // set 1 byte register or memory if parity (if even)
  X86_SETS,           // set 1 byte register or memory if sign
  X86_STC,            // set carry flag
  X86_STD,            // set direction flag
  X86_XADD,           // exchange and add register or memory
  X86_XCHG,           // exchange register or memory

  X86_CLFLUSH,    // clear all L1/L2... caches containing memory address. [CPUID CLFSH]
  X86_CLFLUSHOPT, // clear all L1/L2... caches containing memory address. [CPUID CLFLUSHOPT]
  X86_CLWB,       // write-back cache line containing memory address. [CPUID CLWB]

  // the following instructions require [CPUID RTM]
  X86_XABORT, // abort TSX transaction with 1 byte immediate value
  X86_XBEGIN, // start TSX transaction, with relative address of code to run on abort

};

enum OpStmt2 : uint16_t {
  BAD_ST2 = 0,
  // intentionally same values as Op2
  ADD_ASSIGN,     // +=
  SUB_ASSIGN,     // -=
  MUL_ASSIGN,     // *=
  QUO_ASSIGN,     // /=
  REM_ASSIGN,     // %=
  AND_ASSIGN,     // &=
  OR_ASSIGN,      // |=
  XOR_ASSIGN,     // ^=
  SHL_ASSIGN,     // <<=
  SHR_ASSIGN,     // >>=
  AND_NOT_ASSIGN, // &^=
  ASSIGN,         // =

  CASE,
  DEFAULT,
  JUMP_IF,

  ASM_MOV = 32, // copy register to register
  ASM_LOAD,     // load register from memory
  ASM_STOR,     // store register to memory

  X86_ADD = 256,  // x += y on register or memory
  X86_ADC,        // x += y + carry on register or memory
  X86_AND,        // bitwise AND (i.e. x&y) register or memory
  X86_BSF,        // x = bitscan_forward(y)
  X86_BSR,        // x = bitscan_reverse(y)
  X86_BT,         // bit test
  X86_BTC,        // bit test and complement
  X86_BTR,        // bit test and reset
  X86_BTS,        // bit test and set
  X86_CMOVA,      // conditional move if above
  X86_CMOVAE,     // conditional move if above or equal (if not carry)
  X86_CMOVB,      // conditional move if below (if carry)
  X86_CMOVBE,     // conditional move if below or equal
  X86_CMOVE,      // conditional move if equal (if zero)
  X86_CMOVG,      // conditional move if greater
  X86_CMOVGE,     // conditional move if greater or equal
  X86_CMOVL,      // conditional move if less
  X86_CMOVLE,     // conditional move if less or equal
  X86_CMOVNE,     // conditional move if not equal (if not zero)
  X86_CMOVNO,     // conditional move if not overflow
  X86_CMOVNP,     // conditional move if not parity (if odd)
  X86_CMOVNS,     // conditional move if not sign
  X86_CMOVO,      // conditional move if overflow
  X86_CMOVP,      // conditional move if parity (if even)
  X86_CMOVS,      // conditional move if sign
  X86_CMP,        // compare two arguments, set EFLAGS
  X86_CMPXCHG,    // compare and exchange 1, 2, 4 or 8 bytes
  X86_CMPXCHG8B,  // compare and exchange 8 bytes
  X86_CMPXCHG16B, // compare and exchange 16 bytes
  X86_IMUL2,      // signed multiply
  X86_LEA,        // load effective address
  X86_LODS,       // load string from ds:%rsi into %al/%ax/%eax/%rax
  X86_MOV,        // general purpose move register, memory or immediate
  X86_MOVNTI,     // store 4 or 8 bytes register to memory, with non-temporal hint
  X86_MOVS,       // move 1,2,4 or 8 bytes from mem at ds:%rsi to mem at es:%rdi, and update both
  X86_MOVSX,      // sign-extend register or memory
  X86_MOVZX,      // zero-extend register or memory
  X86_OR,         // bitwise OR (i.e. x|y) register or memory
  X86_RCL,        // rotate left 1,2,4 or 8 bytes + carry by specified # bits
  X86_RCR,        // rotate right 1,2,4 or 8 bytes + carry by specified # bits
  X86_ROL,        // rotate left 1,2,4 or 8 bytes by specified # bits
  X86_ROR,        // rotate right 1,2,4 or 8 bytes by specified # bits
  X86_SAR,        // right shift register or memory by specified # bits, sign fill
  X86_SHL,        // left shift register or memory by specified # bits
  X86_SHR,        // right shift register or memory by specified # bits, zero fill
  X86_SBB,        // subtract with borrow, register or memory
  X86_SUB,        // subtract register or memory
  X86_TEST,       // bitwise AND register or memory, discard result, set EFLAGS
  X86_XOR,        // bitwise OR (i.e. x^y) register or memory

  // the following instructions require [CPUID SSE]
  X86_MOVHLPD, // move high 8 bytes to low 8 bytes of packed float
  X86_MOVHPD,  // move high 8 bytes to high 8 bytes of packed double
  X86_MOVLHPD, // move low 8 bytes to high 8 bytes of packed double
  X86_MOVLPD,  // move low 8 bytes to low 8 bytes of packed double
  X86_MOVHLPS, // move high 8 bytes to low 8 bytes of packed float
  X86_MOVHPS,  // move high 8 bytes to high 8 bytes of packed float
  X86_MOVLHPS, // move low 8 bytes to high 8 bytes of packed float
  X86_MOVLPS,  // move low 8 bytes to low 8 bytes of packed float

  // the following instructions require [CPUID SSE2]
  X86_CVTSD2SI, // convert double to int
  X86_CVTSD2SS, // convert double to float
  X86_CVTSI2SD, // convert int to double
  X86_CVTSI2SS, // convert int to float
  X86_CVTSS2SD, // convert double to float
  X86_CVTSS2SI, // convert float to int
  X86_DIVSD,    // divide double
  X86_DIVSS,    // divide float
  X86_MAXPD,    // maximum of two %xmm packed double
  X86_MAXPS,    // maximum of two %xmm packed float
  X86_MAXSD,    // maximum double
  X86_MAXSS,    // maximum float
  X86_MINPD,    // minimum of two %xmm packed double
  X86_MINPS,    // minimum of two %xmm packed float
  X86_MINSD,    // minimum double
  X86_MINSS,    // minimum float
  X86_MOVAPD,   // move packed double
  X86_MOVAPS,   // move packed float
  X86_MOVD,     // move 4 bytes between %xmm and register or memory
  X86_MOVDQA,   // move packed int between %xmm and %xmm or aligned memory
  X86_MOVDQU,   // move packed int between %xmm and %xmm or unaligned memory
  X86_MOVMKSPD, // extract the two sign bits from packed double
  X86_MOVMKSPS, // extract the four sign bits from packed float
  X86_MOVNTDQ,  // store packed int to aligned memory, with non-temporal hint
  X86_MOVNTPD,  // store packed double to aligned memory, with non-temporal hint
  X86_MOVNTPS,  // store packed float to aligned memory, with non-temporal hint
  X86_MOVQ,     // move 8 bytes between %xmm and %xmm or memory
  X86_MOVSD,    // move low double from %xmm to %xmm or memory
  X86_PAND,     // bitwise AND of %xmm and %xmm or memory
  X86_PANDN,    // bitwise AND-NOT of %xmm and %xmm or memory
  X86_POR,      // bitwise OR of %xmm and %xmm or memory
  X86_PXOR,     // bitwise XOR of %xmm and %xmm or memory

  // the following instructions require [CPUID SSE3]
  X86_LDDQU, // load unaligned 128 bits into xmm

  // the following instructions require [CPUID SSE4.1]
  X86_MOVNTDQA, // load packed double from aligned memory, with non-temporal hint

  // the following instructions require [CPUID SSE4.2]
  X86_CRC32, // accumulate CRC32c of 1, 2, 4 or 8 bytes

  // the following instructions require [CPUID LZCNT]
  X86_LZCNT, // count most significant zero bits

  // the following instructions require [CPUID MOVBE]
  X86_MOVBE, // move and byte-swap register

  // the following instructions require [CPUID POPCNT]
  X86_POPCNT, // count bits = 1
};

enum OpStmt3 : uint16_t {
  BAD_ST3 = 0,
  IF = 1,

  ASM_CMP = 32, // compare two arguments, set architectural flags

  X86_DIV = 256, // unsigned divide %rdx:%rax by argument
  X86_IDIV,      // signed divide %rdx:%rax by argument
  X86_IMUL3,     // signed multiply by constant
  X86_MUL,       // unsigned multiply %rdx:%rax by argument
  X86_SHLD, // left shift register or memory by specified # bits, with fill from another register
  X86_SHRD, // right shift register or memory by specified # bits, with fill from another register

  // the following instructions require [CPUID SSE2]
  X86_PEXTRW, // extract 4 bytes from %xmm to register

  // the following instructions require [CPUID SSE4.1]
  X86_EXTRACTPS, // extract one float from packed floats
  X86_INSERTPS,  // insert one float into packed floats
  X86_PINSR,     // insert 1,2,4 or 8 bytes from register or memory to %xmm
};

enum OpStmt4 : uint16_t {
  BAD_ST4 = 0,
  FOR = 1,
};

enum OpStmtN : uint16_t {
  BAD_STN = 0,
  ASSIGN_CALL = 1,
  BLOCK = 2,
  COND = 3, // n-ary IF ... [ELSEIF* ... [ELSE ...]]
  RETURN = 4,
  SWITCH = 5,
};

const Chars &to_string(OpStmt0 op) noexcept;
const Chars &to_string(OpStmt1 op) noexcept;
const Chars &to_string(OpStmt2 op) noexcept;
const Chars &to_string(OpStmt3 op) noexcept;
const Chars &to_string(OpStmt4 op) noexcept;
const Chars &to_string(OpStmtN op) noexcept;

const Fmt &operator<<(const Fmt &out, OpStmt0 op);
const Fmt &operator<<(const Fmt &out, OpStmt1 op);
const Fmt &operator<<(const Fmt &out, OpStmt2 op);
const Fmt &operator<<(const Fmt &out, OpStmt3 op);
const Fmt &operator<<(const Fmt &out, OpStmt4 op);
const Fmt &operator<<(const Fmt &out, OpStmtN op);

} // namespace onejit

#endif // ONEJIT_OPSTMT_HPP

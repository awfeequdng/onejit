/*
 * oneasm - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
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
 * inst.hpp
 *
 *  Created on Jan 26, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEASM_X86_INST_HPP
#define ONEASM_X86_INST_HPP

#include <oneasm/x86/arg.hpp>
#include <oneasm/x86/reg.hpp>
#include <onejit/opstmt.hpp>
#include <onestl/chars.hpp>

namespace oneasm {
namespace x86 {

////////////////////////////////////////////////////////////////////////////////
// describe x86/amd64 instruction constraints and side effects
class Inst {
public:
  constexpr explicit Inst(BitSize val_size = B0, Eflags eflags = EFnone) noexcept
      : val_size_{val_size}, eflags_{eflags} {
  }

  constexpr Eflags eflags() const {
    return eflags_;
  }

  constexpr BitSize val_size() const noexcept {
    return val_size_;
  }

private:
  BitSize val_size_; // allowed immediate sizes
  Eflags eflags_;    // reads or writes eflags?
};

////////////////////////////////////////////////////////////////////////////////
// no-argument x86 instruction
class Inst0 : public Inst {
  using Base = Inst;
  using Chars = onestl::Chars;

public:
  template <uint8_t N>
  constexpr explicit Inst0(const char (&chars)[N],
                           Eflags eflags = EFnone) noexcept //
      : Base{B0, eflags},                                   //
        chars_len_{uint8_t(N - 1)},                         //
        chars_{
            chars[0],
            N >= 2 ? chars[1] : '\0',
            N >= 3 ? chars[2] : '\0',
        } {
  }

  constexpr const Chars chars() const noexcept {
    return Chars{chars_, chars_len_};
  }

private:
  uint8_t chars_len_;
  char chars_[3];
};

const Inst0 &to_inst(onejit::OpStmt0 op);
bool emit(onestl::CharBuf &dst, const Inst0 &inst0);

////////////////////////////////////////////////////////////////////////////////
// one-argument x86 instruction
class Inst1 : public Inst {
  using Base = Inst;

public:
  constexpr explicit Inst1(Arg1 arg, BitSize arg_size, BitSize val_size = B0,
                           Eflags eflags = EFnone) noexcept
      : Base{val_size, eflags}, arg_{arg}, arg_size_{arg_size} {
  }

  constexpr Arg1 arg() const noexcept {
    return arg_;
  }

  constexpr BitSize arg_size() const noexcept {
    return arg_size_;
  }

private:
  Arg1 arg_;         // allowed argument combinations
  BitSize arg_size_; // allowed argument sizes
};

const Inst1 &to_inst(onejit::OpStmt1 op);
bool emit(onestl::CharBuf &dst, const Inst1 &inst1);

////////////////////////////////////////////////////////////////////////////////
// two-arguments x86 instruction
class Inst2 : public Inst {
  using Base = Inst;

public:
  constexpr explicit Inst2(Arg2 arg, BitSize val_size = B0, Eflags eflags = EFnone) noexcept
      : Base{val_size, eflags}, arg_{arg} {
  }

  constexpr Arg2 arg() const noexcept {
    return arg_;
  }

private:
  Arg2 arg_; // allowed argument combinations
};

////////////////////////////////////////////////////////////////////////////////
// three-arguments x86 instruction
class Inst3 : public Inst {
  using Base = Inst;

public:
  constexpr explicit Inst3(Arg3 arg, BitSize val_size = B0, Eflags eflags = EFnone) noexcept
      : Base{val_size, eflags}, arg_{arg} {
  }

  constexpr Arg3 arg() const noexcept {
    return arg_;
  }

private:
  Arg3 arg_; // allowed argument combinations
};

} // namespace x86
} // namespace oneasm

#endif // ONEASM_X86_INST_HPP

/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * inst.hpp
 *
 *  Created on Jan 26, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_INST_HPP
#define ONEJIT_X64_INST_HPP

#include <onejit/opstmt.hpp>
#include <onejit/x64/arg.hpp>
#include <onejit/x64/fwd.hpp>
#include <onejit/x64/regid.hpp>
#include <onestl/view.hpp>

namespace onejit {
namespace x64 {

////////////////////////////////////////////////////////////////////////////////
// describe x86/x64 instruction constraints and side effects
class Inst {
public:
  constexpr explicit Inst(BitSize imm_size = B0, Eflags eflags = EFnone) noexcept
      : imm_size_{imm_size}, eflags_{eflags} {
  }

  constexpr Eflags eflags() const {
    return eflags_;
  }

  constexpr BitSize imm_size() const noexcept {
    return imm_size_;
  }

private:
  BitSize imm_size_; // allowed immediate sizes
  Eflags eflags_;    // reads or writes eflags?
};

////////////////////////////////////////////////////////////////////////////////
// no-argument x86 instruction
class Inst0 : public Inst {
  using Base = Inst;

public:
  template <uint8_t N>
  constexpr explicit Inst0(const char (&chars)[N],
                           Eflags eflags = EFnone) noexcept //
      : Base{B0, eflags},                                   //
        bytes_len_{uint8_t(N - 1)},                         //
        bytes_{
            uint8_t(chars[0]),
            uint8_t(N >= 2 ? chars[1] : '\0'),
            uint8_t(N >= 3 ? chars[2] : '\0'),
        } {
  }

  constexpr const Bytes bytes() const noexcept {
    return Bytes{bytes_, bytes_len_};
  }

private:
  uint8_t bytes_len_;
  uint8_t bytes_[3];
};

////////////////////////////////////////////////////////////////////////////////
// one-argument x86 instruction
class Inst1 : public Inst {
  using Base = Inst;

public:
  template <size_t N>
  constexpr Inst1(const char (&chars)[N],   //
                  const char imm8_chars[],  //
                  const char imm32_chars[], //
                  Arg1 arg,                 //
                  BitSize arg_size,         //
                  BitSize imm_size = B0,    //
                  Eflags eflags = EFnone) noexcept
      : Base{imm_size, eflags}, //
        bytes_len_{uint8_t(N > 0 ? N - 1 : 0)},
        bytes_{
            uint8_t(N > 0 ? chars[0] : '\0'), //
            uint8_t(N > 1 ? chars[1] : '\0'), //
            uint8_t(N > 2 ? chars[2] : '\0'), //
        },                                    //
        imm8_bytes_{
            uint8_t(imm8_chars[0]),                        //
            uint8_t(imm8_chars[0] ? imm8_chars[1] : '\0'), //
        },                                                 //
        imm32_bytes_{
            uint8_t(imm32_chars[0]),                         //
            uint8_t(imm32_chars[0] ? imm32_chars[1] : '\0'), //
        },                                                   //
        arg_{arg}, arg_size_{arg_size} {
  }

  /// @return instruction prefix to use when argument is register or memory
  constexpr Bytes bytes() const noexcept {
    return Bytes{bytes_, bytes_len_};
  }

  /// @return instruction prefix to use when argument is 8-bit immediate
  constexpr Bytes imm8_bytes() const noexcept {
    return Bytes{imm8_bytes_, size_t((imm8_bytes_[0] != 0) + (imm8_bytes_[1] != 0))};
  }

  /// @return instruction prefix to use when argument is 32-bit immediate or label
  constexpr Bytes imm32_bytes() const noexcept {
    return Bytes{imm32_bytes_, size_t((imm32_bytes_[0] != 0) + (imm32_bytes_[1] != 0))};
  }

  constexpr Arg1 arg() const noexcept {
    return arg_;
  }

  constexpr BitSize arg_size() const noexcept {
    return arg_size_;
  }

private:
  uint8_t bytes_len_;
  uint8_t bytes_[3];
  uint8_t imm8_bytes_[2];
  uint8_t imm32_bytes_[2];
  Arg1 arg_;         // allowed argument combinations
  BitSize arg_size_; // allowed argument sizes
};

////////////////////////////////////////////////////////////////////////////////
// two-arguments x86 instruction
class Inst2 : public Inst {
  using Base = Inst;

public:
  constexpr explicit Inst2(Arg2 arg, BitSize imm_size = B0, Eflags eflags = EFnone) noexcept
      : Base{imm_size, eflags}, arg_{arg} {
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
  constexpr explicit Inst3(Arg3 arg, BitSize imm_size = B0, Eflags eflags = EFnone) noexcept
      : Base{imm_size, eflags}, arg_{arg} {
  }

  constexpr Arg3 arg() const noexcept {
    return arg_;
  }

private:
  Arg3 arg_; // allowed argument combinations
};

////////////////////////////////////////////////////////////////////////////////
// N-argument x86 instruction
class InstN : public Inst {
  using Base = Inst;

public:
  template <uint8_t N>
  constexpr explicit InstN(const char (&chars)[N],
                           Eflags eflags = EFnone) noexcept //
      : Base{B0, eflags},                                   //
        bytes_len_{uint8_t(N - 1)},                         //
        bytes_{
            uint8_t(chars[0]),
            uint8_t(N >= 2 ? chars[1] : '\0'),
            uint8_t(N >= 3 ? chars[2] : '\0'),
        } {
  }

  constexpr const Bytes bytes() const noexcept {
    return Bytes{bytes_, bytes_len_};
  }

private:
  uint8_t bytes_len_;
  uint8_t bytes_[3];
};

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_INST_HPP

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
 * imm.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IMM_HPP
#define ONEJIT_IMM_HPP

#include <onejit/node.hpp>
#include <onejit/value.hpp>

namespace onejit {

// immediate constant
class Imm : public Value {
  using Base = Value;

  friend class Func;
  friend class Node;
  friend class Const;

public:
  // construct an invalid imm.
  constexpr Imm() noexcept //
      : Base{}, direct_{} {
  }
  constexpr explicit Imm(const Value &val) noexcept //
      : Base{val}, direct_{Direct{Base::bits(), ekind(), 0}} {
  }
  constexpr explicit Imm(bool val) noexcept //
      : Base{val}, direct_{Direct{uint16_t(val), ekind()}} {
  }
  constexpr explicit Imm(int8_t val) noexcept //
      : Base{val}, direct_{Direct{int16_t(val), ekind()}} {
  }
  constexpr explicit Imm(int16_t val) noexcept //
      : Base{val}, direct_{Direct{int16_t(val), ekind()}} {
  }
  constexpr explicit Imm(int32_t val) noexcept //
      : Base{val}, direct_{Direct{Base::bits(), ekind(), 0}} {
  }
  constexpr explicit Imm(int64_t val) noexcept //
      : Base{val}, direct_{Direct{Base::bits(), ekind(), 0}} {
  }
  constexpr explicit Imm(uint8_t val) noexcept //
      : Base{val}, direct_{Direct{uint16_t(val), ekind()}} {
  }
  constexpr explicit Imm(uint16_t val) noexcept //
      : Base{val}, direct_{Direct{uint16_t(val), ekind()}} {
  }
  constexpr explicit Imm(uint32_t val) noexcept //
      : Base{val}, direct_{Direct{Base::bits(), ekind(), 0}} {
  }
  constexpr explicit Imm(uint64_t val) noexcept //
      : Base{val}, direct_{Direct{Base::bits(), ekind(), 0}} {
  }
  constexpr explicit Imm(float val) noexcept //
      : Base{val}, direct_{Direct{Base::bits(), ekind(), 0}} {
  }
  constexpr explicit Imm(double val) noexcept //
      : Base{val}, direct_{Direct{Base::bits(), ekind(), 0}} {
  }
  constexpr Imm(Kind kind, uint64_t bits) noexcept //
      : Base{kind, bits}, direct_{Direct{Base::bits(), ekind(), 0}} {
  }

  static constexpr Type type() noexcept {
    return CONST;
  }

  Imm cast(Kind to) noexcept;

private:
  class Direct;

  constexpr Imm(uint64_t bits, Kind kind, Direct direct) noexcept //
      : Base{kind, bits}, direct_{direct} {
  }

  class Direct {
    friend class Imm;

  public:
    constexpr explicit Direct(uint32_t data) noexcept : val_{data} {
    }

    // bits is only 16 bits, and only low 4 bits of ekind are used
    // => is always direct
    constexpr Direct(uint16_t bits, eKind ekind) noexcept //
        : val_{make_direct(ekind, uint64_t(bits), 0, 0)} {
    }

    // bits is only 16 bits, and only low 4 bits of ekind are used
    // => is always direct
    constexpr Direct(int16_t bits, eKind ekind) noexcept //
        : val_{make_direct(ekind, uint64_t(bits), 0, uint64_t(-int64_t(bits < 0)))} {
    }

    // create direct value if possible, otherwise zero
    constexpr Direct(uint64_t bits, eKind ekind, int) noexcept
        // Kind must fit 4 bits
        : val_{(ekind & 0xF0) != 0 ? 0 : recurse(ekind, bits, 0, 0)} {
    }

    constexpr Kind kind() const noexcept {
      return Kind((val_ >> 1) & 0xF);
    }

    constexpr operator uint32_t() const noexcept {
      return val_;
    }

    constexpr Imm to_const() const noexcept {
      return Imm{xor_mask() ^ lrot(imm(), rotation_bits()), kind(), *this};
    }

  private:
    constexpr uint64_t xor_mask() const noexcept {
      return -uint64_t(val_ >> 31);
    }

    constexpr uint64_t imm() const noexcept {
      return (val_ >> 8) & 0x7FFFFF;
    }

    constexpr uint64_t rotation_bits() const noexcept {
      return (val_ & 0xE0) >> 2;
    }

    static constexpr uint64_t lrot(uint64_t val, uint8_t rotate_bits) noexcept {
      return rotate_bits == 0 ? val : (val << rotate_bits) | (val >> (64 - rotate_bits));
    }

    static constexpr uint64_t rrot(uint64_t val, uint8_t rotate_bits) noexcept {
      return rotate_bits == 0 ? val : (val >> rotate_bits) | (val << (64 - rotate_bits));
    }

    // return direct value if possible, otherwise zero
    static constexpr ONEJIT_NOINLINE uint32_t recurse(eKind ekind, uint64_t bits, //
                                                      uint8_t rotate_bits,
                                                      uint64_t xor_mask) noexcept {
      return can_direct(ekind, bits, rotate_bits, xor_mask)                               //
                 ? make_direct(ekind, bits, rotate_bits, xor_mask)                        //
                 : xor_mask == 0 ? recurse(ekind, bits, rotate_bits, ~xor_mask)           //
                                 : rotate_bits < 64                                       //
                                       ? recurse(ekind, bits, rotate_bits + 8, ~xor_mask) //
                                       : 0;
    }

    static constexpr bool can_direct(eKind /*ekind*/, uint64_t bits, //
                                     uint8_t rotate_bits, uint64_t xor_mask) noexcept {
      return rrot(bits ^ xor_mask, rotate_bits) <= 0x7FFFFF;
    }

    static constexpr uint32_t make_direct(eKind ekind, uint64_t bits, //
                                          uint8_t rotate_bits, uint64_t xor_mask) noexcept {
      return 1 | (ekind & 0x0F) << 1                             //
             | uint32_t(rotate_bits / 8) << 5                    //
             | uint32_t(rrot(bits ^ xor_mask, rotate_bits)) << 8 //
             | uint32_t(xor_mask & (1 << 31));
    }

  private:
    uint32_t val_;
  };

  constexpr bool is_direct() const noexcept {
    return direct_ != 0;
  }

  // usable only if is_direct() returns true
  constexpr uint32_t direct() const noexcept {
    return direct_;
  }

  static constexpr Kind parse_direct_kind(uint32_t data) noexcept {
    return Direct{data}.kind();
  }

  static constexpr Imm parse_direct(uint32_t data) noexcept {
    return Direct{data}.to_const();
  }

  static Imm parse_indirect(Kind kind, Offset offset, const Code *holder) noexcept;

  Code &write_indirect(Code *holder) const noexcept;

  uint32_t direct_;
};

} // namespace onejit

#endif // ONEJIT_IMM_HPP

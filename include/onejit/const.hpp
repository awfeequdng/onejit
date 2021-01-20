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
 * const.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_CONST_HPP
#define ONEJIT_CONST_HPP

#include <onejit/node.hpp>

#include <iosfwd>

namespace onejit {

union ConstFloat32 {
public:
  constexpr ConstFloat32() noexcept : float_{0.0f} {
  }
  constexpr explicit ConstFloat32(float val) noexcept : float_{val} {
  }
  constexpr explicit ConstFloat32(uint32_t bits) noexcept : bits_{bits} {
  }

  constexpr float val() const noexcept {
    return float_;
  }
  constexpr uint32_t bits() const noexcept {
    return bits_;
  }

private:
  float float_;
  uint32_t bits_;
}; // namespace onejit

////////////////////////////////////////////////////////////////////////////////

union ConstFloat64 {
public:
  constexpr ConstFloat64() noexcept : float_{0.0} {
  }
  constexpr explicit ConstFloat64(double val) noexcept : float_{val} {
  }
  constexpr explicit ConstFloat64(uint64_t bits) noexcept : bits_{bits} {
  }

  constexpr double val() const noexcept {
    return float_;
  }
  constexpr uint64_t bits() const noexcept {
    return bits_;
  }

private:
  double float_;
  uint64_t bits_;
}; // namespace onejit

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

class Const {
  friend class Func;
  friend class Node;
  friend class ConstExpr;

public:
  // construct a Void constant. equivalent to C/C++ source "(void)0"
  constexpr Const() noexcept //
      : bits_{}, ekind_{kVoid}, direct_{Direct{uint16_t(0), ekind_}} {
  }
  constexpr explicit Const(bool val) noexcept //
      : bits_{uint64_t(val)}, ekind_{kBool}, direct_{Direct{uint16_t(val), ekind_}} {
  }
  constexpr explicit Const(int8_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{kInt8}, direct_{Direct{int16_t(val), ekind_}} {
  }
  constexpr explicit Const(int16_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{kInt16}, direct_{Direct{int16_t(val), ekind_}} {
  }
  constexpr explicit Const(int32_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{kInt32}, direct_{Direct{bits_, ekind_, 0}} {
  }
  constexpr explicit Const(int64_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{kInt64}, direct_{Direct{bits_, ekind_, 0}} {
  }
  constexpr explicit Const(uint8_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{kUint8}, direct_{Direct{uint16_t(val), ekind_}} {
  }
  constexpr explicit Const(uint16_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{kUint16}, direct_{Direct{uint16_t(val), ekind_}} {
  }
  constexpr explicit Const(uint32_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{kUint32}, direct_{Direct{bits_, ekind_, 0}} {
  }
  constexpr explicit Const(uint64_t val) noexcept //
      : bits_{uint64_t(val)}, ekind_{kUint64}, direct_{Direct{bits_, ekind_, 0}} {
  }
  constexpr explicit Const(float val) noexcept //
      : bits_{ConstFloat32{val}.bits()}, ekind_{kFloat32}, direct_{Direct{bits_, ekind_, 0}} {
  }
  constexpr explicit Const(double val) noexcept //
      : bits_{ConstFloat64{val}.bits()}, ekind_{kFloat64}, direct_{Direct{bits_, ekind_, 0}} {
  }
  constexpr Const(Kind kind, uint64_t bits) noexcept //
      : bits_{bits}, ekind_{kind.val()}, direct_{Direct{bits_, ekind_, 0}} {
  }

  constexpr Type type() const noexcept {
    return CONST;
  }

  constexpr Kind kind() const noexcept {
    return Kind{ekind_};
  }

  constexpr explicit operator bool() const noexcept {
    return ekind_ != kVoid;
  }

  constexpr bool boolean() const noexcept {
    return bool(bits_);
  }

  constexpr int8_t int8() const noexcept {
    return int8_t(bits_);
  }
  constexpr int16_t int16() const noexcept {
    return int16_t(bits_);
  }
  constexpr int32_t int32() const noexcept {
    return int32_t(bits_);
  }
  constexpr int64_t int64() const noexcept {
    return int64_t(bits_);
  }

  constexpr uint8_t uint8() const noexcept {
    return uint8_t(bits_);
  }
  constexpr uint16_t uint16() const noexcept {
    return uint16_t(bits_);
  }
  constexpr uint32_t uint32() const noexcept {
    return uint32_t(bits_);
  }
  constexpr uint64_t uint64() const noexcept {
    return uint64_t(bits_);
  }

  constexpr float float32() const noexcept {
    return ConstFloat32{uint32_t(bits_)}.val();
  }
  constexpr double float64() const noexcept {
    return ConstFloat64{bits_}.val();
  }

  void *ptr() const noexcept {
    return reinterpret_cast<void *>(size_t(bits_));
  }

private:
  class Direct;

  constexpr Const(uint64_t bits, Kind kind, Direct direct) noexcept //
      : bits_{bits}, ekind_{kind.val()}, direct_{direct} {
  }

  class Direct {
    friend class Const;

  public:
    constexpr explicit Direct(uint32_t data) noexcept : val_(data) {
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
    constexpr ONEJIT_NOINLINE Direct(uint64_t bits, eKind ekind, int) noexcept
        // Kind must fit 4 bits
        : val_{(ekind & 0xF0) != 0 ? 0 : recurse(ekind, bits, 0, 0)} {
    }

    constexpr Kind kind() const noexcept {
      return Kind((val_ >> 1) & 0xF);
    }

    constexpr operator uint32_t() const noexcept {
      return val_;
    }

    constexpr Const to_const() const noexcept {
      return Const{xor_mask() ^ lrot(imm(), rotation_bits()), kind(), *this};
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
    static constexpr uint32_t recurse(eKind ekind, uint64_t bits, //
                                      uint8_t rotate_bits, uint64_t xor_mask) noexcept {
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

  static constexpr Const parse_direct(uint32_t data) noexcept {
    return Direct{data}.to_const();
  }

  static Const parse_indirect(Kind kind, Offset offset, const Code *holder) noexcept;

  Code &write_indirect(Code *holder) const noexcept;

  uint64_t bits_;
  eKind ekind_;
  uint32_t direct_;
};

constexpr const Const True{true};
constexpr const Const False{false};

constexpr inline bool operator==(Const a, Const b) noexcept {
  return a.kind() == b.kind() && a.uint64() == b.uint64();
}

constexpr inline bool operator!=(Const a, Const b) noexcept {
  return a.kind() != b.kind() || a.uint64() != b.uint64();
}

std::ostream &operator<<(std::ostream &out, const Const &c);

} // namespace onejit

#endif // ONEJIT_CONST_HPP

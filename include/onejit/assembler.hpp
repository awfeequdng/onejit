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
 * code.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ASSEMBLER_HPP
#define ONEJIT_ASSEMBLER_HPP

#include <onejit/error.hpp>
#include <onejit/node/label.hpp>
#include <onestl/buffer.hpp>

namespace onejit {

class Assembler : public Buffer<uint8_t> {
  using T = uint8_t;
  using Base = Buffer<T>;

public:
  constexpr Assembler() noexcept : Base{} {
  }

  explicit Assembler(size_t capacity) noexcept : Base{capacity} {
  }

  ~Assembler() noexcept;

  using Base::operator bool;
  using Base::begin;
  using Base::clear;
  using Base::data;
  using Base::end;
  using Base::size;
  using Base::span;
  using Base::truncate;
  using Base::view;

  /**
   * high-level methods, they assemble symbolic instructions
   */

  // assemble an x86_64 instruction. defined in onejit/x64/asm.cpp
  Assembler &x64(const Node &node) noexcept;

  /**
   * low-level methods, they add raw bytes
   */

  Assembler &add(T byte) noexcept {
    return add(View<T>{&byte, 1});
  }

  Assembler &add(const T *bytes, size_t len) noexcept {
    return add(View<T>{bytes, len});
  }

  Assembler &add(View<T> bytes) noexcept {
    return static_cast<Assembler &>(Base::append(bytes));
  }

  Assembler &add(std::initializer_list<T> bytes) noexcept {
    return add(View<T>{bytes.begin(), bytes.size()});
  }

  Bytes bytes() const noexcept {
    return Bytes{*this};
  }

  // mark last added bytes to be filled with label relative offset
  // does nothing if label is invalid i.e. bool(l) == false
  Assembler &add_relocation(Label l) noexcept;

  constexpr Errors get_errors() const noexcept {
    return error_;
  }

  // add an assembler error
  Assembler &error(const Node &where, Chars msg) noexcept;

  // add an out of memory error
  Assembler &out_of_memory(const Node &where) noexcept;

private:
  // hide Base::append()
  void append(...) noexcept;

  Vector<Relocation> relocation_;
  Vector<Error> error_;

}; // class Assembler

} // namespace onejit

#endif // ONEJIT_ASSEMBLER_HPP

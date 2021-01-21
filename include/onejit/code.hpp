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
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_CODE_HPP
#define ONEJIT_CODE_HPP

#include <onejit/const.hpp>
#include <onejit/fwd.hpp>
#include <onejit/node.hpp>
#include <onejit/type.hpp>
#include <onestl/bytes.hpp>
#include <onestl/vector.hpp>

#include <cstdint> // uint32_t

namespace onejit {

class Code : private Vector<CodeItem> {
  friend class Node;
  typedef CodeItem T;
  typedef Vector<T> Base;

public:
  Code() noexcept;
  explicit Code(size_t capacity) noexcept;
  ~Code() noexcept;

  constexpr explicit operator bool() const noexcept {
    return good_;
  }

  // checked element access:
  // throws if byte_offset is out of bounds
  const T &at(Offset byte_offset) const {
    return Base::at(byte_offset / sizeof(T));
  }

  // checked element access:
  // returns 0 if byte_offset is out of bounds
  T get(Offset byte_offset) const noexcept {
    const size_t index = byte_offset / sizeof(T);
    return index < Base::size() ? Base::operator[](index) : T{};
  }

  // returns 0 if byte_offset is out of bounds
  int32_t int32(Offset byte_offset) const noexcept {
    return int32_t(get(byte_offset));
  }
  // returns 0 if byte_offset is out of bounds
  uint32_t uint32(Offset byte_offset) const noexcept {
    return get(byte_offset);
  }
  // returns 0 if byte_offset is out of bounds
  float float32(Offset byte_offset) const noexcept {
    return ConstFloat32{get(byte_offset)}.val();
  }

  // returns 0 if byte_offset is out of bounds
  int64_t int64(Offset byte_offset) const noexcept {
    return int64_t(uint64(byte_offset));
  }
  // returns 0 if byte_offset is out of bounds
  uint64_t uint64(Offset byte_offset) const noexcept;
  // returns 0 if byte_offset is out of bounds
  double float64(Offset byte_offset) const noexcept {
    return ConstFloat64{uint64(byte_offset)}.val();
  }

  Code &add(int32_t i32) noexcept {
    return add(uint32_t(i32));
  }
  Code &add(uint32_t u32) noexcept; // same as add(CodeItem)
  Code &add(float f32) noexcept {
    return add(ConstFloat32{f32}.bits());
  }

  Code &add(int64_t i64) noexcept {
    return add(uint64_t(i64));
  }
  Code &add(uint64_t u64) noexcept;
  Code &add(double f64) noexcept {
    return add(ConstFloat64{f64}.bits());
  }

  Code &add(CodeItems data) noexcept;
  Code &add(NodeHeader header) noexcept {
    return add(header.item());
  }
  Code &add(const Kind kind) noexcept;
  Code &add(const Kinds kinds) noexcept;
  Code &add(const Node &node, Offset parent_offset) noexcept;
  Code &add(Nodes nodes, Offset parent_offset) noexcept;

  /// \return Code length, in bytes
  constexpr Offset length() const noexcept {
    return Base::size() * sizeof(T);
  }

  /// truncate Code to specified number of bytes
  void truncate(Offset length_bytes) noexcept {
    length_bytes /= sizeof(T);
    if (size_ > length_bytes) {
      size_ = length_bytes;
    }
  }

  // return Code length, in CodeItems
  using Base::size;

  constexpr const T *data() const noexcept {
    return Base::data();
  }
  constexpr const T *begin() const noexcept {
    return Base::begin();
  }
  constexpr const T *end() const noexcept {
    return Base::end();
  }

private:
  bool good_;
};

} // namespace onejit

#endif // ONEJIT_CODE_HPP

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

  Code &add_int32(int32_t i32) noexcept {
    return add_item(uint32_t(i32));
  }
  Code &add_uint32(uint32_t u32) noexcept {
    return add_item(u32);
  }
  Code &add_float32(float f32) noexcept {
    return add_item(ConstFloat32{f32}.bits());
  }

  Code &add_int64(int64_t i64) noexcept {
    return add_uint64(uint64_t(i64));
  }
  Code &add_uint64(uint64_t u64) noexcept;
  Code &add_float64(double f64) noexcept {
    return add_uint64(ConstFloat64{f64}.bits());
  }

  Code &add_item(CodeItem data) noexcept; // same as add_uint32()
  Code &add(CodeItems data) noexcept;
  Code &add(NodeHeader header) noexcept {
    return add_item(header.item());
  }
  Code &add(const Kind kind) noexcept;
  Code &add(const Kinds kinds) noexcept;
  Code &add(const Node &node, Offset parent_offset) noexcept;

  // NODE_T must be a subclass of Node
  template <class NODE_T>
  Code &ONEJIT_NOINLINE add(const View<NODE_T> nodes, Offset parent_offset) noexcept {
    for (const Node &node : nodes) {
      if (!add(node, parent_offset)) {
        break;
      }
    }
    return *this;
  }

  // return Code length, in CodeItems
  using Base::size;

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
  Code &init() noexcept;

  bool good_;
};

} // namespace onejit

#endif // ONEJIT_CODE_HPP

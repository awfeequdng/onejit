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
  Code();
  explicit Code(size_t capacity);
  ~Code();

  constexpr explicit operator bool() const {
    return good_;
  }

  const T &at(Offset byte_offset) const {
    return Base::operator[](byte_offset / sizeof(T));
  }

  int32_t i32(Offset byte_offset) const {
    return int32_t(at(byte_offset));
  }
  uint32_t uint32(Offset byte_offset) const {
    return at(byte_offset);
  }
  int64_t int64(Offset byte_offset) const {
    return int64_t(uint64(byte_offset));
  }
  uint64_t uint64(Offset byte_offset) const;
  float float32(Offset byte_offset) const;
  double float64(Offset byte_offset) const;

  Code &add(CodeItem item);
  Code &add(CodeView data);
  Code &add(NodeHeader header) {
    return add(header.item());
  }
  Code &add(const Node &node, Offset parent_offset) {
    // we save relative offset between parent and child:
    // makes it easier to concatenate different Code objects
    return add(node.offset_or_direct() - (node.is_direct() ? 0 : parent_offset));
  }
  Code &add(int32_t i32) {
    return add(uint32_t(i32));
  }
  // Code &add(uint32_t val); // same as add(CodeItem) above
  Code &add(float f32) {
    const union {
      float f32;
      uint32_t u32;
    } x = {f32};
    return add(x.u32);
  }

  Code &add(int64_t i64) {
    return add(uint64_t(i64));
  }
  Code &add(uint64_t u64);
  Code &add(double f64) {
    const union {
      double f64;
      uint64_t u64;
    } x = {f64};
    return add(x.u64);
  }

  /// \return Code length, in bytes
  constexpr Offset length() const {
    return Base::size() * sizeof(T);
  }

  /// truncate Code to specified number of bytes
  void truncate(Offset length_bytes) {
    length_bytes /= sizeof(T);
    if (size_ > length_bytes) {
      size_ = length_bytes;
    }
  }

  // return Code length, in CodeItems
  using Base::size;

  constexpr const T *data() const {
    return Base::data();
  }
  constexpr const T *begin() const {
    return Base::begin();
  }
  constexpr const T *end() const {
    return Base::end();
  }

private:
  bool good_;
};

} // namespace onejit

#endif // ONEJIT_CODE_HPP

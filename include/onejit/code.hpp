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

#include <onejit/bytes.hpp>
#include <onejit/fwd.hpp>
#include <onejit/type.hpp>
#include <onejit/vector.hpp>

#include <cstdint> // uint32_t

namespace onejit {

class Code : private Vector<CodeItem> {
  friend class Node;
  typedef Vector<CodeItem> Base;

public:
  Code();
  explicit Code(size_t capacity);
  ~Code();

  constexpr explicit operator bool() const {
    return good_;
  }

  const CodeItem &at(Offset byte_offset) const;
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

  Code &add(CodeView data);
  Code &add(int32_t val) {
    return add(uint32_t(val));
  }
  Code &add(uint32_t val);
  Code &add(int64_t val) {
    return add(uint64_t(val));
  }
  Code &add(uint64_t val);
  Code &add(float val);
  Code &add(double val);

private:
  bool good_;
};

} // namespace onejit

#endif // ONEJIT_CODE_HPP

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
 * code.cpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#include "onejit/code.hpp"
#include "onejit/nodeheader.hpp"

#include <cstring>

namespace onejit {

Code::Code() : Base(), good_(true) {
}

Code::Code(size_t capacity) : Base(), good_(true) {
  good_ &= reserve(capacity);
}

Code::~Code() {
  static_assert(sizeof(NodeHeader) == 4, "sizeof(NodeHeader) must be 4");
  static_assert(sizeof(CodeItem) == 4, "sizeof(CodeItem) must be 4");
  static_assert(sizeof(uint32_t) == 4, "sizeof(uint32_t) must be 4");
  static_assert(sizeof(uint64_t) == 8, "sizeof(uint32_t) must be 8");
  static_assert(sizeof(float) == 4, "sizeof(double) must be 4");
  static_assert(sizeof(double) == 8, "sizeof(double) must be 8");
}

const uint32_t &Code::at(Offset byte_offset) const {
  return Base::operator[](byte_offset / sizeof(uint32_t));
}

uint64_t Code::uint64(Offset byte_offset) const {
  const uint32_t *addr = &at(byte_offset + 4) - 1;
  uint64_t val;
  std::memcpy(&val, addr, sizeof(val));
  return val;
}

float Code::float32(Offset byte_offset) const {
  const uint32_t *addr = &at(byte_offset);
  float val;
  std::memcpy(&val, addr, sizeof(val));
  return val;
}

double Code::float64(Offset byte_offset) const {
  const uint32_t *addr = &at(byte_offset + 4) - 1;
  double val;
  std::memcpy(&val, addr, sizeof(val));
  return val;
}

Code & ONEJIT_NOINLINE Code::add(const CodeView data) {
  good_ = good_ && Base::append(data);
  return *this;
}

Code &Code::add(uint32_t val) {
  return add(CodeView(&val, 1));
}

Code &Code::add(uint64_t val) {
  uint32_t v[2];
  std::memcpy(v, &val, sizeof(v));
  return add(CodeView(v, 2));
}

Code &Code::add(float val) {
  uint32_t v[1];
  std::memcpy(v, &val, sizeof(v));
  return add(CodeView(v, 1));
}

Code &Code::add(double val) {
  uint32_t v[2];
  std::memcpy(v, &val, sizeof(v));
  return add(CodeView(v, 2));
}

} // namespace onejit

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

#include <cstring>

namespace onejit {

Code::Code() : Base() {
}

Code::Code(size_t capacity) : Base() {
  reserve(capacity);
}

Code::~Code() {
}

uint8_t Code::get8(Offset offset) const {
  return at(offset);
}
uint16_t Code::get16(Offset offset) const {
  const uint8_t *addr = &at(offset + 1) - 1;
  uint16_t val;
  std::memcpy(&val, addr, sizeof(val));
  return val;
}
uint32_t Code::get32(Offset offset) const {
  const uint8_t *addr = &at(offset + 3) - 3;
  uint32_t val;
  std::memcpy(&val, addr, sizeof(val));
  return val;
}
uint64_t Code::get64(Offset offset) const {
  const uint8_t *addr = &at(offset + 7) - 7;
  uint64_t val;
  std::memcpy(&val, addr, sizeof(val));
  return val;
}

void Code::add(const Bytes data) {
  Base::insert(Base::end(), data.begin(), data.end());
}

void Code::add8(uint8_t val) {
  add(Bytes(&val, sizeof(val)));
}
void Code::add16(uint16_t val) {
  uint8_t v[2];
  memcpy(v, &val, sizeof(v));
  add(Bytes(v, sizeof(v)));
}
void Code::add32(uint32_t val) {
  uint8_t v[4];
  memcpy(v, &val, sizeof(v));
  add(Bytes(v, sizeof(v)));
}
void Code::add64(uint64_t val) {
  uint8_t v[8];
  memcpy(v, &val, sizeof(v));
  add(Bytes(v, sizeof(v)));
}

} // namespace onejit

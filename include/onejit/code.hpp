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
#include <onejit/offset.hpp>
#include <onejit/type.hpp>

#include <cstdint> // uint8_t
#include <vector>

namespace onejit {

class Arg;

class Code : private std::vector<uint8_t> {
  friend class Arg;
  typedef std::vector<uint8_t> Base;

public:
  Code();
  explicit Code(size_t capacity);
  ~Code();

  uint8_t get8(Offset offset) const;
  uint16_t get16(Offset offset) const;
  uint32_t get32(Offset offset) const;
  uint64_t get64(Offset offset) const;

  void add(Bytes data);
  void add8(uint8_t val);
  void add16(uint16_t val);
  void add32(uint32_t val);
  void add64(uint64_t val);

private:
};

} // namespace onejit

#endif // ONEJIT_CODE_HPP

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
 * type.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_TYPE_HPP
#define ONEJIT_TYPE_HPP

#include <cstdint> // uint8_t
#include <iosfwd>  // std::ostream

namespace onejit {

// intentionally match Go go/token.Token values
enum Type : uint8_t {
  BAD = 0,
  UNARY = 1,
  BINARY = 2,
  TUPLE = 3,
  BREAK = 61,
  CASE = 62,
  CONTINUE = 65,
  DEFAULT = 66,
  FALLTHROUGH = 69,
  FOR = 70,
  IF = 74,
  SWITCH = 73,
};

class Chars;
const Chars &to_string(Type op);

std::ostream &operator<<(std::ostream &out, Type op);

} // namespace onejit

#endif // ONEJIT_TYPE_HPP

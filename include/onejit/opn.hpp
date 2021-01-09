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
 * opn.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPN_HPP
#define ONEJIT_OPN_HPP

#include <cstdint> // uint8_t
#include <iosfwd>  // std::ostream

namespace onejit {

// intentionally match Go go/token.Token values
enum OpN : uint8_t {
  CALL = 54, // RPAREN
  RETURN = 80,
};

class Chars;
const Chars &to_string(OpN op);

std::ostream &operator<<(std::ostream &out, OpN op);

} // namespace onejit

#endif // ONEJIT_OPN_HPP

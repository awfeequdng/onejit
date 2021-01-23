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
 * error.hpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ERROR_HPP
#define ONEJIT_ERROR_HPP

#include <onejit/fwd.hpp>
#include <onejit/node.hpp>
#include <onestl/chars.hpp>

namespace onejit {

// a compile error, with Node that caused it and error message
class Error {
public:
  constexpr Error() noexcept : where_{}, msg_{} {
  }

  constexpr Error(const Node &where, Chars msg) noexcept : where_{where}, msg_{msg} {
  }

  ~Error() noexcept = default;

  constexpr Node where() const noexcept {
    return where_;
  }

  constexpr Chars msg() const noexcept {
    return msg_;
  }

private:
  Node where_;
  Chars msg_;
};

#endif // ONEJIT_ERROR_HPP

} // namespace onejit

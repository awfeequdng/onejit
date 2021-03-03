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
 * error.cpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/error.hpp>
#include <onejit/fmt.hpp>
#include <onestl/crange.hpp>
#include <onestl/view.hpp>

namespace onejit {

const Fmt &operator<<(const Fmt &fmt, const Error &error) {
  return fmt << error.msg() << ": " << error.where();
}

const Fmt &operator<<(const Fmt &fmt, const Errors &errors) {
  if (errors) {
    fmt << "errors:\n";
    for (const Error &err : errors) {
      fmt << "    " << err << '\n';
    }
  }
  return fmt;
}

const Fmt &operator<<(const Fmt &fmt, const CRange<Error> &errors) {
  if (errors) {
    fmt << "errors:\n";
    for (size_t i = 0, n = errors.size(); i < n; i++) {
      fmt << "    " << errors[i] << '\n';
    }
  }
  return fmt;
}

} // namespace onejit

/*
 * onestl - Tiny STL C++ library
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
 * check.cpp
 *
 *  Created on Jan 14, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/check.hpp>
#include <onestl/fmt.hpp>
#include <onestl/string.hpp>

#include <stdexcept>

namespace onestl {

Failed::Failed(String &&lhs, String &&rhs, const char *opstr, const char *lstr, const char *rstr,
               const char *file, int line) noexcept
    : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(opstr), lstr_(lstr), rstr_(rstr), file_(file),
      line_(line) {
}

void ONESTL_NORETURN Failed::throw_check_failed() const {
  String buf;
  Fmt{&buf} << "check failed at " << file_ << ':' << line_          //
            << "\n\tcheck: " << lstr_ << ' ' << op_ << ' ' << rstr_ //
            << "\n\twhere  " << lstr_ << "\tis " << lhs_            //
            << "\n\tand    " << rstr_ << "\tis " << rhs_;

  throw std::range_error(buf.c_str());
}

void ONESTL_NORETURN Failed::throw_bounds_failed() const {
  String buf;
  Fmt{&buf} << "container out-of-bounds access at " << file_ << ':' << line_ //
            << "\n\tcheck: " << lstr_ << ' ' << op_ << ' ' << rstr_          //
            << "\n\twhere  " << lstr_ << "\tis " << lhs_                     //
            << "\n\tand    " << rstr_ << "\tis " << rhs_;

  throw std::out_of_range(buf.c_str());
}

void ONESTL_NORETURN throw_check_failed() {
  throw std::range_error("runtime check failed");
}

void ONESTL_NORETURN throw_bounds_failed() {
  throw std::out_of_range("container out-of-bounds access");
}

} // namespace onestl

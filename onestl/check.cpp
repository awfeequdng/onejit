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
 *  Created on Jan 14, 2020
 *      Author Massimiliano Ghilardi
 */

#include <onestl/chars.hpp>
#include <onestl/check.hpp>

#include <stdexcept>
#include <string>

namespace onestl {

std::string to_string(std::nullptr_t) {
  return std::string("nullptr", 7);
}

std::string to_string(bool val) {
  return val ? std::string("true", 4) : std::string("false", 5);
}

std::string to_string(int8_t val) {
  return to_string(int(val));
}

std::string to_string(uint8_t val) {
  return to_string(unsigned(val));
}

std::string to_string(const void *val) {
  std::stringstream buf;
  buf.write("0x", 2);
  buf << std::hex << size_t(val);
  return buf.str();
}

Error::Error(std::string &&lhs, std::string &&rhs, const char *opstr, const char *lstr,
             const char *rstr, const char *file, int line) noexcept
    : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(opstr), lstr_(lstr), rstr_(rstr), file_(file),
      line_(line) {
}

void ONESTL_NORETURN Error::throw_check_failed() const {
  std::stringstream buf;
  buf << Chars("check failed at ") << file_ << ':' << line_          //
      << Chars("\n\tcheck: ") << lstr_ << ' ' << op_ << ' ' << rstr_ //
      << Chars("\n\twhere  ") << lstr_ << Chars("\tis ") << lhs_     //
      << Chars("\n\tand    ") << rstr_ << Chars("\tis ") << rhs_;

  throw std::range_error(buf.str());
}

void ONESTL_NORETURN Error::throw_bounds_failed() const {
  std::stringstream buf;
  buf << Chars("container out-of-bounds access at ") << file_ << ':' << line_ //
      << Chars("\n\tcheck: ") << lstr_ << ' ' << op_ << ' ' << rstr_          //
      << Chars("\n\twhere  ") << lstr_ << Chars("\tis ") << lhs_              //
      << Chars("\n\tand    ") << rstr_ << Chars("\tis ") << rhs_;

  throw std::out_of_range(buf.str());
}

void ONESTL_NORETURN throw_check_failed() {
  throw std::range_error("runtime check failed");
}

void ONESTL_NORETURN throw_bounds_failed() {
  throw std::out_of_range("container out-of-bounds access");
}

} // namespace onestl

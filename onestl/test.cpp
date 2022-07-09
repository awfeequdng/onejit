/*
 * onestl - Tiny STL C++ library
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test.cpp
 *
 *  Created on Jan 14, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onestl/io/fmt.hpp>
#include <onestl/string.hpp>
#include <onestl/test.hpp>

#include <stdexcept>

namespace onestl {

Failed::Failed(String &&lhs, String &&rhs, const char *opstr, const char *lstr, const char *rstr,
               const char *file, int line) noexcept
    : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(opstr), lstr_(lstr), rstr_(rstr), file_(file),
      line_(line) {
}

void ONESTL_NORETURN Failed::throw_check_failed() const {
  String buf;
  Fmt{&buf} << "check failed at " << Chars::of(file_) << ':' << line_ //
            << "\n\tcheck: " << Chars::of(lstr_) << ' ' << Chars::of(op_) << ' '
            << Chars::of(rstr_)                                     //
            << "\n\twhere  " << Chars::of(lstr_) << "\tis " << lhs_ //
            << "\n\tand    " << Chars::of(rstr_) << "\tis " << rhs_;

  throw std::runtime_error(buf.c_str());
}

void ONESTL_NORETURN Failed::throw_bounds_failed() const {
  String buf;
  Fmt{&buf} << "container out-of-bounds access at " << Chars::of(file_) << ':' << line_ //
            << "\n\tcheck: " << Chars::of(lstr_) << ' ' << Chars::of(op_) << ' '
            << Chars::of(rstr_)                                     //
            << "\n\twhere  " << Chars::of(lstr_) << "\tis " << lhs_ //
            << "\n\tand    " << Chars::of(rstr_) << "\tis " << rhs_;

  throw std::out_of_range(buf.c_str());
}

void ONESTL_NORETURN throw_check_failed() {
  throw std::runtime_error("runtime check failed");
}

void ONESTL_NORETURN throw_bounds_failed() {
  throw std::out_of_range("container out-of-bounds access");
}

} // namespace onestl

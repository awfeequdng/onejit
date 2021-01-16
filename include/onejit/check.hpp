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
 * check.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_CHECK_HPP
#define ONEJIT_CHECK_HPP

#include <iosfwd>
#include <sstream>

#ifdef __OPTIMIZE__
#define check(lhs, op, rhs)
#else
#define check(lhs, op, rhs) CHECK(lhs, op, rhs)
#endif

#define CHECK(lhs, op, rhs)                                                                        \
  do {                                                                                             \
    if ((lhs) /**/ op /**/ (rhs)) {                                                                \
      break;                                                                                       \
    }                                                                                              \
    ::onejit::CheckFailed(lhs, rhs, #lhs, #op, #rhs);                                              \
  } while (false);

namespace onejit {

class CheckFailed {
public:
  template <class T1, class T2>
  CheckFailed(const T1 &lhs, const T2 &rhs, const char *lstr, const char *opstr, const char *rstr)
      : lhs_(lstr), op_(opstr), rhs_(rstr) {
    std::stringstream lval, rval;
    lval << lhs;
    rval << rhs;
    lval_ = lval.str().c_str();
    rval_ = rval.str().c_str();
    dothrow();
  }

private:
  void
#ifdef __GNUC__
      __attribute__((noreturn))
#endif
      dothrow();

  const char *lval_;
  const char *rval_;
  const char *lhs_;
  const char *op_;
  const char *rhs_;
};

} // namespace onejit

namespace std {
std::ostream &operator<<(std::ostream &, std::nullptr_t);
}

#endif /* ONEJIT_CHECK_HPP */

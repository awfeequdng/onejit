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

#include <onejit/fwd.hpp>

#include <ostream>
#include <sstream>

#ifdef __OPTIMIZE__
#define check(lhs, op, rhs)
#else
#define check(lhs, op, rhs) CHECK(lhs, op, rhs)
#endif

#define CHECK(lhs, op, rhs) (((lhs) /**/ op /**/ (rhs)) ? (void)0 : CHECK_FAILED(lhs, op, rhs))

#define CHECK_FAILED(lhs, op, rhs)                                                                 \
  ::onejit::CheckFailed(lhs, rhs, #lhs, #op, #rhs, __FILE__, __LINE__).throw_error()

namespace onejit {

class CheckFailed {
public:
  template <class T1, class T2>
  CheckFailed(const T1 &lhs, const T2 &rhs,                          //
              const char *lstr, const char *opstr, const char *rstr, //
              const char *file, int line)
      : lhs_(lstr), op_(opstr), rhs_(rstr), file_(file), line_(line) {

    print(lval_, lhs);
    print(rval_, rhs);
  }

  void ONEJIT_NORETURN throw_error() const;

private:
  template <class T> static void print(std::string &out, const T &val) {
    std::stringstream buf;
    buf << val;
    out = buf.str();
  }

  template <class T> static void print(std::string &out, const T *val) {
    std::stringstream buf;
    buf.write("0x", 2);
    buf << std::hex << size_t(val);
    out = buf.str();
  }

  static void print(std::string &out, std::nullptr_t) {
    out.assign("nullptr", 7);
  }

  std::string lval_;
  std::string rval_;
  const char *lhs_;
  const char *op_;
  const char *rhs_;
  const char *file_;
  int line_;
};

} // namespace onejit

#endif /* ONEJIT_CHECK_HPP */

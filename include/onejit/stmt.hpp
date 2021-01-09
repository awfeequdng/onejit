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
 * stmt.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_STMT_HPP
#define ONEJIT_STMT_HPP

#include <onejit/arg.hpp>

namespace onejit {

class Stmt0 {
public:
  constexpr Stmt0() : type_(BAD) {
  }

  constexpr type() const {
    return type_;
  }

protected:
  constexpr explicit Stmt0(Type t) : type_(t) {
  }

private:
  Type type_;
};

////////////////////////////////////////////////////////////////////////////////
class Stmt1 : public Arg {
public:
  constexpr Stmt1() : Arg() {
  }

protected:
  constexpr Stmt1(Code *code, Offset offset) //
      : Arg(code, offset) {
  }
};

////////////////////////////////////////////////////////////////////////////////
class Break : Stmt0 {
public:
  Break() : Stmt0(BREAK) {
  }
};

////////////////////////////////////////////////////////////////////////////////
class Continue : Stmt0 {
public:
  Continue() : Stmt0(CONTINUE) {
  }
};

////////////////////////////////////////////////////////////////////////////////
class Fallthrough : Stmt0 {
public:
  Fallthrough() : Stmt0(FALLTHROUGH) {
  }
};

} // namespace onejit

#endif // ONEJIT_STMT_HPP

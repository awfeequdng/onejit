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
 * arg.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ARG_HPP
#define ONEJIT_ARG_HPP

#include <onejit/code.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
class Arg {
public:
  constexpr Arg() : code_(nullptr), offset_(0) {
  }

  Type type() const {
    return code_ == nullptr ? BAD : Type(code_->get8(offset_));
  }

  uint8_t children() const;
  Arg child(uint8_t i) const;

protected:
  constexpr Arg(Code *code, Offset offset) //
      : code_(code), offset_(offset) {
  }

  constexpr Code *code() const {
    return code_;
  }

  constexpr Offset offset() const {
    return offset_;
  }

private:
  Code *code_;
  Offset offset_;
};

} // namespace onejit

#endif // ONEJIT_ARG_HPP
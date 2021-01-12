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
 * nodeheader.hpp
 *
 *  Created on Jan 12, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODEHEADER_HPP
#define ONEJIT_NODEHEADER_HPP

#include <onejit/fwd.hpp>
#include <onejit/kind.hpp>
#include <onejit/type.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// first CodeItem contained in BinaryExpr, UnaryExpr, Stmt*
class NodeHeader {
  friend class Node;

public:
  constexpr NodeHeader() : val_(BAD) {
  }

  constexpr explicit NodeHeader(CodeItem item) : val_(item) {
  }

  constexpr Type type() const {
    return Type(val_ & 0xF);
  }

  constexpr Kind kind() const {
    return Kind(val_ >> 4);
  }

  constexpr uint32_t children() const {
    return val_ >> 8;
  }

private:
  constexpr CodeItem val() const {
    return val_;
  }

  CodeItem val_;
};

} // namespace onejit

#endif // ONEJIT_ARG_HPP

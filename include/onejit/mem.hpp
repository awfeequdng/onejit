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
 * mem.hpp
 *
 *  Created on Jan 16, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_MEM_HPP
#define ONEJIT_MEM_HPP

#include <onejit/expr.hpp>
#include <onejit/fmt.hpp>
#include <onejit/op.hpp>

namespace onejit {

// keep track of Mem subclasses
enum MemType : uint16_t {
  GENERIC_MEM = 0,
  X64_MEM = 1,
  ARM64_MEM = 2,
};

// an unary expression: a memory dereference, either read or write.
// the memory address is an expression
class Mem : public Expr {
  using Base = Expr;

  friend class Func;
  friend class Node;

public:
  /**
   * construct an invalid Mem.
   * exists only to allow placing Mem in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Mem, use one of the other constructors
   */
  constexpr Mem() noexcept : Base{} {
  }

  Mem(Func &func, Kind kind, Expr address) noexcept //
      : Base{create(func, kind, GENERIC_MEM, address)} {
  }

  static constexpr Type type() noexcept {
    return MEM;
  }

  using Base::kind;

  constexpr MemType memtype() const noexcept {
    return MemType(Base::op());
  }

  static constexpr uint32_t children() noexcept {
    return 1;
  }

  // shortcut for child(0).is<Expr>()
  Expr addr() const noexcept {
    return child(0).is<Expr>();
  }

protected:
  // used by subclasses
  Mem(Func &func, Kind kind, MemType memtype, Expr address) noexcept //
      : Base{create(func, kind, memtype, address)} {
  }

  // downcast Node to Mem. also used by subclasses
  constexpr explicit Mem(const Node &node) noexcept : Base{node} {
  }

private:
  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == MEM;
  }

  static Node create(Func &func, Kind kind, MemType memtype, Expr address) noexcept;
};

const Chars to_string(MemType memtype) noexcept;
const Fmt &operator<<(const Fmt &out, MemType memtype);
const Fmt &operator<<(const Fmt &out, const Mem &mem);

} // namespace onejit

#endif // ONEJIT_MEM_HPP

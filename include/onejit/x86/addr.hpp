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
 * addr.hpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_X86_ADDR_HPP
#define ONEJIT_X86_ADDR_HPP

#include <onejit/label.hpp>
#include <onejit/tuple.hpp>
#include <onejit/var.hpp>

namespace onejit {
namespace x86 {

enum class Scale : uint8_t {
  None = 0,
  S0 = 0,
  S1 = 1,
  S2 = 2,
  S4 = 4,
  S8 = 8,
};

////////////////////////////////////////////////////////////////////////////////
// address representable in a single x86 instruction:
// optional_label + offset_int32 + optional_base_register + optional_index_reg * {1,2,4,8}
class Addr : public Tuple {
  using Base = Tuple;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Addr.
   * exists only to allow placing Addr in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Addr, use one of the other constructors
   */
  constexpr Addr() noexcept : Base{} {
  }

  Addr(Func &func, Kind kind,    //
       const int32_t offset = 0, //
       const Var &base = Var{},  //
       const Var &index = Var{}, //
       Scale scale = Scale::None) noexcept
      : Base{create(func, kind, Label{}, offset, base, index, scale)} {
  }

  Addr(Func &func, const Label &label, //
       const int32_t offset = 0,       //
       const Var &base = Var{},        //
       const Var &index = Var{},       //
       Scale scale = Scale::None) noexcept
      : Base{create(func, Ptr, label, offset, base, index, scale)} {
  }

  static constexpr OpN op() noexcept {
    return X86_ADDR;
  }

  // shortcut for child(0).is<Label>()
  Label label() const noexcept;

  // shortcut for child(1).is<Const>().imm().int32()
  int32_t offset() const noexcept;

  // shortcut for child(2).is<Var>()
  Var base() const noexcept;

  // shortcut for child(3).is<Var>()
  Var index() const noexcept;

  // shortcut for Scale(child(4).is<Const>().imm().int8())
  Scale scale() const noexcept;

private:
  // downcast Node to Addr
  constexpr explicit Addr(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == X86_ADDR;
  }

  static Node create(Func &func, Kind kind, const Label &label, const int32_t offset,
                     const Var &base, const Var &index, Scale scale) noexcept;
}; // class Addr

} // namespace x86
} // namespace onejit

#endif // ONEJIT_X86_ADDR_HPP

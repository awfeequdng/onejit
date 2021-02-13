/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
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
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_MEM_HPP
#define ONEJIT_X64_MEM_HPP

#include <onejit/node/const.hpp>
#include <onejit/node/label.hpp>
#include <onejit/node/mem.hpp>
#include <onejit/node/var.hpp>
#include <onejit/x64/scale.hpp>

namespace onejit {
namespace x64 {

////////////////////////////////////////////////////////////////////////////////
// memory access representable in a single x86 instruction: address is
// optional_label + offset_int32 + optional_base_register + optional_index_reg * {1,2,4,8}
class Mem : public onejit::Mem {
  using Base = onejit::Mem;
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

  Mem(Func &func, Kind kind,    //
      const int32_t offset = 0, //
      const Var &base = Var{},  //
      const Var &index = Var{}, //
      Scale scale = Scale0) noexcept
      : Base{create(func, kind, Label{}, offset, base, index, scale)} {
  }

  Mem(Func &func, const Label &label, //
      const int32_t offset = 0,       //
      const Var &base = Var{},        //
      const Var &index = Var{},       //
      Scale scale = Scale0) noexcept
      : Base{create(func, Ptr, label, offset, base, index, scale)} {
  }

  static constexpr OpN op() noexcept {
    return X86_MEM;
  }

  // shortcut for child_is<Label>(0)
  Label label() const noexcept {
    return child_is<Label>(0);
  }

  // shortcut for child_is<Const>(1).val().int32()
  int32_t offset() const noexcept;

  // shortcut for child_is<Var>(2).local()
  Local base() const noexcept;

  // shortcut for child_is<Var>(3).local()
  Local index() const noexcept;

  // shortcut for Scale(child_is<Const>(4).val().uint8())
  Scale scale() const noexcept;

private:
  // downcast Node to Mem
  constexpr explicit Mem(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == X86_MEM;
  }

  static Node create(Func &func, Kind kind, const Label &label, const int32_t offset,
                     const Var &base, const Var &index, Scale scale) noexcept;

}; // class Mem

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_MEM_HPP

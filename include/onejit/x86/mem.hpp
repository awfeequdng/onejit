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
#ifndef ONEJIT_X86_MEM_HPP
#define ONEJIT_X86_MEM_HPP

#include <onejit/mem.hpp>
#include <onejit/x86/addr.hpp>

namespace onejit {
namespace x86 {

////////////////////////////////////////////////////////////////////////////////
// memory access representable in a single x86 instruction: address is x86::Addr i.e.
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

  Mem(Func &func, Addr address) noexcept //
      : Base{func, address.kind(), X86_MEM, address} {
  }

  static constexpr MemType memtype() noexcept {
    return X86_MEM;
  }

  // shortcut for child(0).is<Addr>()
  Addr address() const noexcept;

private:
  // downcast Node to Mem
  constexpr explicit Mem(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == X86_MEM;
  }
};

const Fmt &operator<<(const Fmt &out, const Mem &mem);

} // namespace x86
} // namespace onejit

#endif // ONEJIT_X86_MEM_HPP

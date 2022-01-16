/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * mem.hpp
 *
 *  Created on Jan 28, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_MIR_MEM_HPP
#define ONEJIT_MIR_MEM_HPP

#include <onejit/ir/const.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/mem.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/mir/fwd.hpp>
#include <onejit/mir/scale.hpp>

namespace onejit {
namespace mir {

////////////////////////////////////////////////////////////////////////////////
// memory access representable in a single MIR instruction: address is
// offset_int64 + optional_base_register + optional_index_reg * {1,2,4,8}
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

  Mem(Func &func, Kind kind, const Address &address) noexcept //
      : Base{create(func, kind, address)} {
  }

  // try to create mir::Mem whose address is the sum of all children.
  // return invalid mir::Mem if it fails.
  Mem(Compiler &comp, Kind kind, Nodes children) noexcept //
      : Base{create(comp, kind, children)} {
  }

  // try to create mir::Mem whose address is the sum of all children.
  // return invalid mir::Mem if it fails.
  Mem(Compiler &comp, Kind kind, const ChildRange &children) noexcept
      : Base{create(comp, kind, children)} {
  }

  static constexpr OpN op() noexcept {
    return MIR_MEM;
  }

  // shortcut for child_is<Label>(0)
  Label label() const noexcept {
    return child_is<Label>(0);
  }

  // shortcut for child_is<Const>(0).val().int64()
  int64_t offset() const noexcept;

  // shortcut for child_is<Var>(1).local()
  Local base() const noexcept;

  // shortcut for child_is<Var>(2).local()
  Local index() const noexcept;

  // shortcut for Scale(child_is<Const>(3).val().uint8())
  Scale scale() const noexcept;

private:
  // downcast Node to Mem
  constexpr explicit Mem(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_op(uint16_t op) noexcept {
    return op == MIR_MEM;
  }

  static Node create(Func &func, Kind kind, const Address &address) noexcept;
  static Node create(Compiler &comp, Kind kind, Nodes children) noexcept;
  static Node create(Compiler &comp, Kind kind, const ChildRange &children) noexcept;

}; // class Mem

} // namespace mir
} // namespace onejit

#endif // ONEJIT_MIR_MEM_HPP

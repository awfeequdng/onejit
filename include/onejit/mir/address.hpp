/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * address.hpp
 *
 *  Created on Jan 04, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_MIR_ADDRESS_HPP
#define ONEJIT_MIR_ADDRESS_HPP

#include <onejit/ir/const.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/mir/fwd.hpp>
#include <onejit/mir/scale.hpp>

namespace onejit {
namespace mir {

////////////////////////////////////////////////////////////////////////////////
// address representable bye a single MIR instruction:
// offset_int64 + optional_base_register + optional_index_reg * {1,2,4,8}
class Address {
public:
  int64_t offset;
  Var base;
  Var index;
  Scale scale;
  bool fail;

  constexpr explicit Address(int64_t offset_ = 0, Var base_ = Var{}, Var index_ = Var{},
                             Scale scale_ = Scale1) noexcept
      : offset{offset_}, base{base_}, index{index_}, scale{scale_}, fail{} {
  }

  // try to incrementally fill Address by adding Node. return false if it fails.
  bool insert(Compiler &comp, Node node) noexcept;

  // try to incrementally fill Address adding the sum of all children.
  // return false if it fails.
  bool insert(Compiler &comp, Exprs children) noexcept;

  // try to incrementally fill Address by adding the sum of all children.
  // return false if it fails.
  bool insert(Compiler &comp, const ChildRange &children) noexcept;

}; // class Address

} // namespace mir
} // namespace onejit

#endif // ONEJIT_MIR_ADDRESS_HPP

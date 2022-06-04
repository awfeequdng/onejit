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
 *  Created on Feb 23, 2021
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_X64_ADDRESS_HPP
#define ONEJIT_X64_ADDRESS_HPP

#include <onejit/ir/const.hpp>
#include <onejit/ir/label.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/x64/scale.hpp>

namespace onejit {
namespace x64 {

////////////////////////////////////////////////////////////////////////////////
// address representable bye a single x86 instruction:
// optional_label + offset_int32 + optional_base_register + optional_index_reg * {1,2,4,8}
class Address {
public:
  Label label;
  Var base;
  Var index;
  Scale scale;
  int32_t offset;
  bool fail;

  constexpr explicit Address(Label label_ = Label{}, int32_t offset_ = 0, Var base_ = Var{},
                             Var index_ = Var{}, Scale scale_ = Scale1) noexcept
      : label{label_}, base{base_}, index{index_}, scale{scale_}, offset{offset_}, fail{} {
  }

  constexpr explicit Address(int32_t offset_, Var base_ = Var{}, Var index_ = Var{},
                             Scale scale_ = Scale1) noexcept
      : label{}, base{base_}, index{index_}, scale{scale_}, offset{offset_}, fail{} {
  }

  // try to incrementally fill Address by adding Node. return false if it fails.
  bool insert(Compiler &comp, Node node) noexcept;

  // try to incrementally fill Address adding the sum of all children.
  /// @return false if it fails.
  bool insert(Compiler &comp, Exprs children) noexcept;

  // try to incrementally fill Address by adding the sum of all children.
  /// @return false if it fails.
  bool insert(Compiler &comp, const ChildRange &children) noexcept;

}; // class Address

} // namespace x64
} // namespace onejit

#endif // ONEJIT_X64_ADDRESS_HPP

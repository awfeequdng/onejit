/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * funcheader.cpp
 *
 *  Created on Mar 15, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/funcheader.hpp>

namespace onejit {

FuncHeader::FuncHeader() noexcept : name_{}, ftype_{}, address_{}, param_n_{}, result_n_{} {
}

FuncHeader &FuncHeader::reset(Name name, FuncType ftype, Expr address) noexcept {
  name_ = name;
  ftype_ = ftype;
  address_ = address;
  param_n_ = ftype.param_n();
  result_n_ = ftype.result_n();
  return *this;
}

} // namespace onejit

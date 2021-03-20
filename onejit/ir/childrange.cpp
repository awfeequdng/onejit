/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * childrange.cpp
 *
 *  Created on Feb 22, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/ir/childrange.hpp>

namespace onejit {
namespace ir {

ChildRange::operator bool() const noexcept {
  uint32_t n = node_.children();
  return node_ && size_ && size_ <= n && start_ <= n - size_;
}

} // namespace ir
} // namespace onejit

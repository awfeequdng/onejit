/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt0.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/ir/stmt0.hpp>

namespace onejit {
namespace ir {

// ============================  Stmt0  ========================================

const Fmt &Stmt0::format(const Fmt &fmt, Syntax /*syntax*/, size_t /*depth*/) const {
  return fmt << op();
}

} // namespace ir
} // namespace onejit

/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * comma.cpp
 *
 *  Created on Feb 17, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/ir/comma.hpp>
#include <onejit/ir/expr.hpp>

namespace onejit {
namespace ir {

// ============================  Comma  ====================================

Node Comma::create(Func &func, Exprs args) noexcept {
  return Base::create(func, Void, COMMA, //
                      Nodes{args.begin(), args.size()});
}

} // namespace ir
} // namespace onejit

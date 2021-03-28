/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * fmt_fwd.hpp
 *
 *  Created on Mar 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_FMT_FWD_HPP
#define ONEJIT_IR_FMT_FWD_HPP

#include <onejit/ir/fwd.hpp>

namespace onejit {
namespace ir {

using Fmt = ::onestl::Fmt;
using Hex = ::onestl::Hex;
using Writer = ::onestl::Writer;

const Fmt &operator<<(const Fmt &fmt, const Node &);

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_FMT_FWD_HPP

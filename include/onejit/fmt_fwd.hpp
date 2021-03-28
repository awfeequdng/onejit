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
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FMT_FWD_HPP
#define ONEJIT_FMT_FWD_HPP

#include <onejit/fwd.hpp>
#include <onejit/ir/fmt_fwd.hpp>

namespace onejit {

using Fmt = ::onestl::Fmt;
using Hex = ::onestl::Hex;
using Writer = ::onestl::Writer;

const Fmt &operator<<(const Fmt &fmt, ArchId);
const Fmt &operator<<(const Fmt &fmt, Bits);
const Fmt &operator<<(const Fmt &fmt, ArchId);
const Fmt &operator<<(const Fmt &fmt, eKind);
const Fmt &operator<<(const Fmt &fmt, const Error &);
const Fmt &operator<<(const Fmt &fmt, const FlowGraph &);
const Fmt &operator<<(const Fmt &fmt, Group);
const Fmt &operator<<(const Fmt &fmt, Kind);
const Fmt &operator<<(const Fmt &fmt, Id);
const Fmt &operator<<(const Fmt &fmt, Local);
const Fmt &operator<<(const Fmt &fmt, Op1);
const Fmt &operator<<(const Fmt &fmt, Op2);
const Fmt &operator<<(const Fmt &fmt, OpN);
const Fmt &operator<<(const Fmt &fmt, OpStmt0);
const Fmt &operator<<(const Fmt &fmt, OpStmt1);
const Fmt &operator<<(const Fmt &fmt, OpStmt2);
const Fmt &operator<<(const Fmt &fmt, OpStmt3);
const Fmt &operator<<(const Fmt &fmt, OpStmt4);
const Fmt &operator<<(const Fmt &fmt, OpStmtN);
const Fmt &operator<<(const Fmt &fmt, const Value &);

} // namespace onejit

#endif // ONEJIT_FMT_HPP

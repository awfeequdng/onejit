/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
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
 * fmt_fwd.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FMT_FWD_HPP
#define ONEJIT_FMT_FWD_HPP

#include <onejit/fwd.hpp>
#include <onejit/node/fmt_fwd.hpp>

namespace onejit {

using Fmt = ::onestl::Fmt;
using Hex = ::onestl::Hex;
using Writer = ::onestl::Writer;

const Fmt &operator<<(const Fmt &fmt, ArchId);
const Fmt &operator<<(const Fmt &fmt, Bits);
const Fmt &operator<<(const Fmt &fmt, ArchId);
const Fmt &operator<<(const Fmt &fmt, eKind);
const Fmt &operator<<(const Fmt &fmt, const Error &);
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

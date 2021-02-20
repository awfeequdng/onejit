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
 * fmt.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_FMT_FWD_HPP
#define ONEJIT_FMT_FWD_HPP

#include <onejit/fwd.hpp>

namespace onejit {

using Fmt = ::onestl::Fmt;
using Hex = ::onestl::Hex;
using Writer = ::onestl::Writer;

const Fmt &operator<<(const Fmt &out, ArchId);
const Fmt &operator<<(const Fmt &out, Bits);
const Fmt &operator<<(const Fmt &out, ArchId);
const Fmt &operator<<(const Fmt &out, eKind);
const Fmt &operator<<(const Fmt &out, Group);
const Fmt &operator<<(const Fmt &out, Kind);
const Fmt &operator<<(const Fmt &out, Id);
const Fmt &operator<<(const Fmt &out, Local);
const Fmt &operator<<(const Fmt &out, const Node &);
const Fmt &operator<<(const Fmt &out, Op1);
const Fmt &operator<<(const Fmt &out, Op2);
const Fmt &operator<<(const Fmt &out, OpN);
const Fmt &operator<<(const Fmt &out, OpStmt0);
const Fmt &operator<<(const Fmt &out, OpStmt1);
const Fmt &operator<<(const Fmt &out, OpStmt2);
const Fmt &operator<<(const Fmt &out, OpStmt3);
const Fmt &operator<<(const Fmt &out, OpStmt4);
const Fmt &operator<<(const Fmt &out, OpStmtN);
const Fmt &operator<<(const Fmt &out, const Value &);

} // namespace onejit

#endif // ONEJIT_FMT_HPP

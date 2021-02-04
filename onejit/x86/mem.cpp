/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
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
 * mem.cpp
 *
 *  Created on Feb 04, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/x86/mem.hpp>

namespace onejit {
namespace x86 {

// shortcut for child(0).is<Addr>()
Addr Mem::address() const noexcept {
  return child(0).is<Addr>();
}

////////////////////////////////////////////////////////////////////////////////

const Fmt &operator<<(const Fmt &out, const Mem &mem) {
  out << '(' << mem.memtype();
  mem.address().format_body(out);
  return out << ')';
}

static const Fmt &format_mem(const Fmt &out, const onejit::Mem &mem) {
  return out << mem.is<Mem>();
}

static const bool registered = Mem::register_formatter(X86_MEM, format_mem);

} // namespace x86
} // namespace onejit

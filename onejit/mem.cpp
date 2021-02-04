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
 * mem.cpp
 *
 *  Created on Jan 20, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/mem.hpp>

namespace onejit {

ONEJIT_NOINLINE Node Mem::create(Func &func, Kind kind, MemType memtype, Expr address) noexcept {
  while (Code *holder = func.code()) {
    const NodeHeader header{MEM, kind, memtype};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add(address, offset)) {
      return Node{header, offset, holder};
    }
    holder->truncate(offset);
    break;
  }
  return Mem{};
}

// shortcut for child(0).is<Expr>()
Expr Mem::address() const noexcept {
  return child(0).is<Expr>();
}

static Mem::formatter_func formatter_vec[memtype_end] = {};

// add a custom formatter_vec for operator<< on Mem subclass
bool Mem::register_formatter(MemType memtype, formatter_func func) noexcept {
  if (memtype < memtype_end) {
    formatter_vec[memtype] = func;
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

static const Chars memclass_string[] = {"mem", "x86_mem", "arm64_mem"};

const Chars to_string(MemType memtype) noexcept {
  size_t i = 0;
  if (memtype < ONEJIT_N_OF(memclass_string)) {
    i = memtype;
  }
  return memclass_string[i];
}

const Fmt &operator<<(const Fmt &out, MemType memtype) {
  return out << to_string(memtype);
}

const Fmt &operator<<(const Fmt &out, const Mem &mem) {
  MemType memtype = mem.memtype();
  if (memtype < memtype_end) {
    if (Mem::formatter_func func = formatter_vec[memtype]) {
      return func(out, mem);
    }
  }
  return out << '(' << memtype << ' ' << mem.child(0) << ')';
}

} // namespace onejit

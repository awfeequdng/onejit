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
 * arg.cpp
 *
 *  Created on Feb 02, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/bits.hpp>
#include <onejit/ir/var.hpp>
#include <onejit/x64/arg.hpp>
#include <onejit/x64/reg.hpp>

namespace onejit {
namespace x64 {

BitSize to_bitsize(Bits bits) noexcept {
  eBits ebits = bits.ebits();
  if (ebits >= eBits8 && ebits <= eBits64) {
    return BitSize(B8 << (ebits - eBits8));
  }
  return B0;
}

Arg1 to_arg(const Node &node) noexcept {
  switch (node.type()) {
  case VAR:
    if (Reg reg = Reg{node.is<Var>().local()}) {
      RegId id = reg.reg_id();
      if (id == RAX) {
        return Arg1::Rax;
      } else if (id == RCX) {
        return Arg1::Rcx;
      } else if ((id >= RAX && id <= R15) || id == RIP) {
        return Arg1::Reg;
      } else if (id >= XMM0 && id <= XMM31) {
        return Arg1::Xmm;
      }
    }
    break;
  case MEM:
    return Arg1::Mem;
  case LABEL:
  case CONST:
    return Arg1::Val;
  default:
    break;
  }
  return Arg1::None;
}

Arg2 to_arg(const Node &node1, const Node &node2) noexcept {
  Arg1 arg1 = to_arg(node1);
  Arg1 arg2 = to_arg(node2);
  if ((arg1 & Arg1::Rax) != Arg1::None) {
    if ((arg2 & Arg1::Rcx) != Arg1::None) {
      return Arg2::Rax_Reg | Arg2::Reg_Rcx;
    } else if ((arg2 & Arg1::Reg) != Arg1::None) {
      return Arg2::Rax_Reg;
    } else if ((arg2 & Arg1::Mem) != Arg1::None) {
      return Arg2::Rax_Mem;
    } else if ((arg2 & Arg1::Val) != Arg1::None) {
      return Arg2::Rax_Val;
    }
  }
  if ((arg1 & Arg1::Reg) != Arg1::None) {
    if ((arg2 & Arg1::Rcx) != Arg1::None) {
      return Arg2::Reg_Rcx;
    } else if ((arg2 & Arg1::Reg) != Arg1::None) {
      return Arg2::Reg_Reg;
    } else if ((arg2 & Arg1::Xmm) != Arg1::None) {
      return Arg2::Reg_Xmm;
    } else if ((arg2 & Arg1::Mem) != Arg1::None) {
      return Arg2::Reg_Mem;
    } else if ((arg2 & Arg1::Val) != Arg1::None) {
      return Arg2::Reg_Val;
    }
  }
  if ((arg1 & Arg1::Xmm) != Arg1::None) {
    if ((arg2 & Arg1::Reg) != Arg1::None) {
      return Arg2::Xmm_Reg;
    } else if ((arg2 & Arg1::Xmm) != Arg1::None) {
      return Arg2::Xmm_Xmm;
    } else if ((arg2 & Arg1::Mem) != Arg1::None) {
      return Arg2::Xmm_Mem;
    }
  }
  if ((arg1 & Arg1::Mem) != Arg1::None) {
    if ((arg2 & Arg1::Rax) != Arg1::None) {
      return Arg2::Mem_Rax;
    } else if ((arg2 & Arg1::Reg) != Arg1::None) {
      return Arg2::Mem_Reg;
    } else if ((arg2 & Arg1::Xmm) != Arg1::None) {
      return Arg2::Mem_Xmm;
    } else if ((arg2 & Arg1::Val) != Arg1::None) {
      return Arg2::Mem_Val;
    }
  }
  return Arg2::None;
}

Arg3 to_arg(const Node &node1, const Node &node2, const Node &node3) noexcept {
  Arg1 arg1 = to_arg(node1);
  Arg1 arg2 = to_arg(node2);
  Arg1 arg3 = to_arg(node3);
  if ((arg1 & Arg1::Reg) != Arg1::None) {
    if ((arg3 & Arg1::Val) != Arg1::None) {
      if ((arg2 & Arg1::Reg) != Arg1::None) {
        return Arg3::Reg_Reg_Val;
      } else if ((arg2 & Arg1::Mem) != Arg1::None) {
        return Arg3::Reg_Mem_Val;
      }
    }
  }
  return Arg3::None;
}

bool is_compatible(Bits bits, BitSize bitsize) noexcept {
  return (to_bitsize(bits) & bitsize) != 0;
}

bool is_compatible(const Node &node1, Arg1 arg) noexcept {
  return (to_arg(node1) & arg) != Arg1::None;
}

bool is_compatible(const Node &node1, const Node &node2, Arg2 arg) noexcept {
  return (to_arg(node1, node2) & arg) != Arg2::None;
}

bool is_compatible(const Node &node1, const Node &node2, const Node &node3, Arg3 arg) noexcept {
  return (to_arg(node1, node2, node3) & arg) != Arg3::None;
}

} // namespace x64
} // namespace onejit

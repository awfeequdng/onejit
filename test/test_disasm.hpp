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
 * test_disasm.hpp
 *
 *  Created on Feb 04, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/check.hpp>
#include <onejit/fwd.hpp>
#include <onejit_config.h> // HAVE_LIBCAPSTONE

#ifdef HAVE_LIBCAPSTONE
#include <capstone.h>
#endif

#define TEST(lhs, op, rhs) (ONEJIT_TEST(lhs, op, rhs), ++testcount(), (void)0)

namespace onejit {

class TestDisasm {
public:
  TestDisasm() noexcept;
  ~TestDisasm() noexcept;

  constexpr explicit operator bool() const noexcept {
    return err_ == 0;
  }

  size_t &testcount() const noexcept {
    return testcount_;
  }

  const Fmt &disasm(const Fmt &out, Bytes bytes);

  // assemble instruction, then disassemble it with capstone
  // and compare its text representation
  void test_asm_disasm_x64(const Node &node, Assembler &assembler);

  static String to_string(const Node &node);

private:
  mutable size_t testcount_;

#ifdef HAVE_LIBCAPSTONE
  const Fmt &format(const Fmt &out, const cs_insn *insn);
  csh handle_;
  cs_err err_;
#else
  int err_;
#endif
};

} // namespace onejit

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

#include <onejit/fwd.hpp>
#include <onejit_config.h> // HAVE_LIBCAPSTONE

#ifdef HAVE_LIBCAPSTONE
#include <capstone.h>
#endif

namespace onejit {

class TestDisasm {
public:
  TestDisasm() noexcept;
  ~TestDisasm() noexcept;

  const Fmt &disasm(const Fmt &out, Bytes bytes);

  constexpr explicit operator bool() const noexcept {
#ifdef HAVE_LIBCAPSTONE
    return err_ == CS_ERR_OK;
#else
    return false;
#endif
  }

private:
#ifdef HAVE_LIBCAPSTONE
  const Fmt &show(const Fmt &out, const cs_insn *insn);

  csh handle_;
  cs_err err_;
#endif
};

} // namespace onejit

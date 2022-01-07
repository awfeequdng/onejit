/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * opstmtn.hpp
 *
 *  Created on Jan 09, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_OPSTMTN_HPP
#define ONEJIT_OPSTMTN_HPP

#include <onejit/fmt.hpp>
#include <onejit/fwd.hpp>

#include <cstdint> // uint16_t

namespace onejit {

enum OpStmtN : uint16_t {
  BAD_STN = 0,
  ASSIGN_CALL = 1, // collect results of a function call and assign them to multiple places
  BLOCK = 2,
  COND = 3, // n-ary IF ... [ELSEIF* ... [ELSE ...]]. allowed only in input IR
  RETURN = 4,
  SWITCH = 5, // children are expression then CASE* and [DEFAULT]. allowed only in input IR

  // numeric values of the OpStmtN enum constants below this line MAY CHANGE WITHOUT WARNING

  SET_ = 6, // arguments are formal registers to set. used in function prologue.

#define ONEJIT_OPSTMTN_MIR(x) /*                                                                */ \
  x(SWITCH, switch)           /* 1st operand is an index, subsequent ops are labels to which goto  \
                                 according the index (1st label has index zero).  The insn         \
                                 behavior is undefined if there is no label for the index. */      \
      x(CALL, call) /* 1st operand is FuncType, then address, then one tuple containing result     \
                       variables, followed by arguments */                                         \
      x(RET, ret)   /* return from function call. arguments are return values  */

#define ONEJIT_OPSTMTN_X86(x) /*                                                                */ \
  x(CALL_, call_) /* call function. 1st argument is destination, others are formal registers */    \
      x(RET, ret) /* return from function call. arguments are formal registers  */

#define ONEJIT_X(NAME, name) MIR_##NAME,
  ONEJIT_OPSTMTN_MIR(ONEJIT_X)
#undef ONEJIT_X

#define ONEJIT_X(NAME, name) X86_##NAME,
      ONEJIT_OPSTMTN_X86(ONEJIT_X)
#undef ONEJIT_X
};

constexpr OpStmtN operator+(OpStmtN op, int delta) noexcept {
  return OpStmtN(int(op) + delta);
}
constexpr OpStmtN operator-(OpStmtN op, int delta) noexcept {
  return OpStmtN(int(op) - delta);
}

const Chars to_string(OpStmtN op) noexcept;

const Fmt &operator<<(const Fmt &fmt, OpStmtN op);

} // namespace onejit

#endif // ONEJIT_OPSTMTN_HPP

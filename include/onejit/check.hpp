/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * check.hpp
 *
 *  Created on Feb 17, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_CHECK_HPP
#define ONEJIT_CHECK_HPP

#include <onejit/ir/allow.hpp>

namespace onejit {

/*
 * Options passed to Compiler::configure() and Optimizer::configure() indicating whether
 * some potentially invalid operation must be always checked or can be optimized away.
 * In case an operation is found to be invalid, it will raise a synchronous signal.
 *
 * For example, DivisionByZero instructs the compiler not to optimize away
 * a division even if its result is unused, because it must raise a signal
 * if the divisor is zero (for example SIGFPE).
 * Note that, even in such case, the compiler can still optimize away a division
 * if it proves *both* that the result is unused and that the divisor is non-zero.
 *
 * Similarly, NullPointerAccess instructs the compiler not to optimize away
 * a memory access even if its result is unused, because it must raise a signal
 * (for example SIGSEGV) if the address is zero, or is derived from a zero address.
 * Also here, the compiler can still optimize away a memory access
 * if it proves *both* that the result is unused and that the address is non-zero.
 */
enum Check : uint8_t {
  CheckNone = 0,
  CheckDivisionByZero = AllowDivision,
  CheckNullPointerAccess = AllowMemAccess,
  CheckAll = 0xff,
};

constexpr inline Check operator~(Check a) noexcept {
  return Check(~unsigned(a));
}

constexpr inline Check operator&(Check a, Check b) noexcept {
  return Check(unsigned(a) & unsigned(b));
}
constexpr inline Check operator|(Check a, Check b) noexcept {
  return Check(unsigned(a) | unsigned(b));
}
constexpr inline Check operator^(Check a, Check b) noexcept {
  return Check(unsigned(a) ^ unsigned(b));
}

inline Check &operator&=(Check &a, Check b) noexcept {
  return a = a & b;
}
inline Check &operator|=(Check &a, Check b) noexcept {
  return a = a | b;
}
inline Check &operator^=(Check &a, Check b) noexcept {
  return a = a ^ b;
}

} // namespace onejit

#endif // ONEJIT_CHECK_HPP

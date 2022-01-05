/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * value_fmt.cpp
 *
 *  Created on Feb 07, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/fmt.hpp>
#include <onejit/value.hpp>
#include <onestl/chars.hpp>

namespace onejit {

const Fmt &operator<<(const Fmt &fmt, const Value &value) {
  const Kind kind = value.kind();
  switch (kind.nosimd().val()) {
  case eVoid:
    fmt << "void";
    break;
  case eBool:
    fmt << (value.boolean() ? Chars("true") : Chars("false"));
    break;
  case eInt8:
  case eInt16:
  case eInt32:
  case eInt64:
    fmt << value.int64();
    break;
  case eUint8:
  case eUint16:
  case eUint32:
  case eUint64:
    fmt << value.uint64();
    break;
  case eFloat32:
    fmt << value.float32();
    break;
  case eFloat64:
  case eFloat128:
    fmt << value.float64();
    break;
  case ePtr:
    fmt << "0x" << Hex{value.ptr()};
    break;
  default:
    fmt << '?';
    break;
  }
  const size_t n = kind.simdn().val();
  if (n > 1) {
    fmt << '_' << kind.stringsuffix() << 'x' << n;
  }
  return fmt;
}

} // namespace onejit

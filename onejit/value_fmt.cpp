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
  return value.format(fmt);
}

const Fmt &Value::format(const Fmt &fmt, ir::Syntax syntax, size_t /*depth*/) const {
  const Kind kind = this->kind();
  switch (kind.nosimd().val()) {
  case eVoid:
    fmt << "void";
    break;
  case eBool:
    fmt << (boolean() ? Chars("true") : Chars("false"));
    break;
  case eInt8:
  case eInt16:
  case eInt32:
  case eInt64:
    fmt << int64();
    break;
  case eUint8:
  case eUint16:
  case eUint32:
  case eUint64:
    fmt << uint64();
    break;
  case eFloat32:
    fmt << float32();
    break;
  case eFloat64:
  case eFloat128:
    fmt << float64();
    break;
  case ePtr:
    fmt << "0x" << Hex{ptr()};
    break;
  default:
    fmt << '?';
    break;
  }
  const size_t n = kind.simdn().val();
  if (syntax == ir::Syntax::Default && n > 1) {
    fmt << '_' << kind.stringsuffix() << 'x' << n;
  }
  return fmt;
}

} // namespace onejit

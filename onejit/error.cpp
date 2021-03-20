/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * error.cpp
 *
 *  Created on Jan 22, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/error.hpp>
#include <onejit/fmt.hpp>
#include <onestl/crange.hpp>
#include <onestl/view.hpp>

namespace onejit {

const Fmt &operator<<(const Fmt &fmt, const Error &error) {
  return fmt << error.msg() << ": " << error.where();
}

const Fmt &operator<<(const Fmt &fmt, const Errors &errors) {
  if (errors) {
    fmt << "errors:\n";
    for (const Error &err : errors) {
      fmt << "    " << err << '\n';
    }
  }
  return fmt;
}

const Fmt &operator<<(const Fmt &fmt, const CRange<Error> &errors) {
  if (errors) {
    fmt << "errors:\n";
    for (size_t i = 0, n = errors.size(); i < n; i++) {
      fmt << "    " << errors[i] << '\n';
    }
  }
  return fmt;
}

} // namespace onejit

/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * codeparser.hpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_CODEPARSER_HPP
#define ONEJIT_CODEPARSER_HPP

#include <onejit/code.hpp>

namespace onejit {

class CodeParser {

public:
  constexpr CodeParser() noexcept : code_(nullptr), offset_(0) {
  }

  constexpr explicit CodeParser(const Code *code) noexcept : code_(code), offset_(0) {
  }

  // ~CodeParser() noexcept = default;

  constexpr explicit operator bool() const noexcept {
    return code_ != nullptr && offset_ < code_->length();
  }

  constexpr Offset tell() const noexcept {
    return offset_;
  }

  void seek(Offset to) noexcept {
    offset_ = to;
  }

  Node next() noexcept;

private:
  const Code *code_;
  Offset offset_;
};

} // namespace onejit

#endif // ONEJIT_CODEPARSER_HPP

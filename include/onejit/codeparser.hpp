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

  // ~CodeParser() = default;

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

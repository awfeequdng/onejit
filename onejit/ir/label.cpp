/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * label.cpp
 *
 *  Created on Jan 21, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/func.hpp>
#include <onejit/ir/label.hpp>

namespace onejit {
namespace ir {

static constexpr uint16_t trivial_hash(uint32_t val) noexcept {
  return uint16_t(val ^ (val >> 16)) | 1;
}

static constexpr uint16_t trivial_hash(uint64_t val) noexcept {
  return trivial_hash(uint32_t(val ^ (val >> 32)));
}

Label::Label(Func &func) noexcept : Base{func.new_label()} {
}

Label Label::create(Code *holder, uint64_t address, uint16_t index) noexcept {
  while (holder) {
    if (!index && address) {
      index = trivial_hash(address);
    }
    const Header header{LABEL, Void, index};
    CodeItem offset = holder->length();

    if (holder->add(header) && holder->add_uint64(address)) {
      return Label{Node{header, offset, holder}};
    }
    holder->truncate(offset);
    break;
  }
  return Label{};
}

const Fmt &Label::format(const Fmt &fmt, Syntax /*syntax*/, size_t /*depth*/) const {
  return fmt << type() << '_' << index();
}

} // namespace ir
} // namespace onejit

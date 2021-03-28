/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * codeparser.cpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/code.hpp>
#include <onejit/codeparser.hpp>
#include <onejit/ir/node.hpp>

namespace onejit {

Node CodeParser::next() noexcept {
  if (offset_ >= code_->length()) {
    return Node{};
  }
  Node node{Header{code_->get(offset_)}, offset_, code_};
  offset_ = add_uint32(offset_, node.length_bytes());
  return node;
}

} // namespace onejit

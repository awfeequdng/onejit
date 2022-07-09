/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * label_hash.hpp
 *
 *  Created on Jun 06, 2022
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_LABEL_HASH_HPP
#define ONEJIT_LABEL_HASH_HPP

#include <onejit/ir/label.hpp>
#include <onejit/ir/node_hash.hpp>

namespace std {
NAMESPACE_NDK_BEGIN

// specialize std::hash<onejit::ir::Node> - allows instantiating
// std::unordered_map<onejit::ir::Node, ...>
template <> struct hash<onejit::ir::Label> : private hash<onejit::ir::Node> {
private:
  typedef hash<onejit::ir::Node> Base;

public:
  typedef onejit::ir::Label argument_type; // deprecated in C++17
  typedef size_t result_type;              // deprecated in C++17

  size_t operator()(onejit::ir::Label key) const noexcept {
    return Base::operator()(key);
  }
};

NAMESPACE_NDK_END
} // namespace std

#endif // ONEJIT_LABEL_HASH_HPP

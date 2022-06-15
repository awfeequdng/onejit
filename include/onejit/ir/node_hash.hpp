/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * node_hash.hpp
 *
 *  Created on Jun 04, 2022
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_NODE_HASH_HPP
#define ONEJIT_NODE_HASH_HPP

#include <onejit/ir/node.hpp>
#include <onejit/ir/hash_fwd.hpp>

#include <functional>

namespace std {
NAMESPACE_NDK_BEGIN

// specialize std::hash<onejit::ir::Node> - allows instantiating
// std::unordered_map<onejit::ir::Node, ...>
template <>
struct hash<onejit::ir::Node> : private hash<onejit::CodeItem>, private hash<const void *> {
private:
  typedef hash<onejit::CodeItem> Base1;
  typedef hash<const void *> Base2;

public:
  typedef onejit::ir::Node argument_type; // deprecated in C++17
  typedef size_t result_type;             // deprecated in C++17

  size_t operator()(onejit::ir::Node key) const noexcept {
    constexpr size_t fnv_prime = sizeof(size_t) <= 4 ? 0x01000193ul : 0x100000001B3ul;
    constexpr size_t fnv_offset = sizeof(size_t) <= 4 ? 0x811c9dc5ul : 0xcbf29ce484222325ul;

    // not an exact fnv implementation: we should xor one byte at time
    size_t ret = fnv_offset;
    ret ^= Base1::operator()(key.header().item());
    ret *= fnv_prime;
    ret ^= Base1::operator()(key.offset_or_direct());
    ret *= fnv_prime;
    ret ^= Base2::operator()(key.code());
    ret *= fnv_prime;
    return ret;
  }
};

NAMESPACE_NDK_END
} // namespace std

#endif // ONEJIT_NODE_HASH_HPP

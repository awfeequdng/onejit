/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * id_hash.hpp
 *
 *  Created on Jun 04, 2022
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_ID_HASH_HPP
#define ONEJIT_ID_HASH_HPP

#include <onejit/id.hpp>

#include <functional>

namespace std {

// specialize std::hash<onejit::Id> - allows instantiating std::unordered_map<onejit::Id, ...>
template <> struct hash<onejit::Id> : private hash<uint32_t> {
  typedef onejit::Id argument_type; // deprecated in C++17
  typedef size_t result_type;       // deprecated in C++17

  size_t operator()(onejit::Id key) const noexcept {
    return hash<uint32_t>::operator()(key.val());
  }
};

} // namespace std

#endif // ONEJIT_ID_HASH_HPP

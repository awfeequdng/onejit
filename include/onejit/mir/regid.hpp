/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * regid.hpp
 *
 *  Created on Jan 04, 2022
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_MIR_REGID_HPP
#define ONEJIT_MIR_REGID_HPP

#include <onejit/archid.hpp>

namespace onejit {
namespace mir {

enum RegId : uint32_t {
  // FIXME:
  // 512...767 are reserved for x64::RegId
  // 768...1023 are reserved for arm64::RegId
  // while MIR register numbers use the full 32-bit range 1...0xffffffff
};

} // namespace mir
} // namespace onejit

#endif // ONEJIT_MIR_REGID_HPP

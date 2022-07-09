/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * scale.hpp
 *
 *  Created on Jan 04, 2022
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_MIR_SCALE_HPP
#define ONEJIT_MIR_SCALE_HPP

#include <onejit/fwd.hpp>

namespace onejit {
namespace mir {

enum Scale : uint8_t {
  Scale0 = 0,
  Scale1 = 1,
  Scale2 = 2,
  Scale4 = 4,
  Scale8 = 8,
};

const Fmt &operator<<(const Fmt &fmt, Scale scale);

} // namespace mir
} // namespace onejit

#endif // ONEJIT_MIR_SCALE_HPP

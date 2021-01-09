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
 * bytes.hpp
 *
 *  Created on Jan 09, 2020
 *      Author Massimiliano Ghilardi
 */
#ifndef ONEJIT_BYTES_HPP
#define ONEJIT_BYTES_HPP

#include <onejit/view.hpp>

#include <cstdint> // uint8_t

namespace onejit {

/** read-only view of uint8_t[] */
typedef View<uint8_t> Bytes;

} // namespace onejit

#endif /* ONEJIT_BYTES_HPP */

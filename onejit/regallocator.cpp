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
 * regallocator.cpp
 *
 *  Created on Feb 26, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/regallocator.hpp>

namespace onejit {

RegAllocator::RegAllocator() noexcept : g_{}, g2_{}, colors_{} {
}

RegAllocator::RegAllocator(size_t num_regs) noexcept //
    : g_{num_regs}, g2_{num_regs}, colors_{num_regs} {

  std::memset(colors_.data(), 0xff, colors_.size() * sizeof(Color));
}

RegAllocator::~RegAllocator() noexcept {
}

bool RegAllocator::reset(size_t num_regs) noexcept {
  if (g_.reset(num_regs) && g2_.reset(num_regs) && colors_.resize(num_regs)) {
    std::memset(colors_.data(), 0xff, colors_.size() * sizeof(Color));
    return true;
  }
  return false;
}

void RegAllocator::allocate_regs(Color num_colors) noexcept {
  g2_.dup(g_); // cannot fail
  for (;;) {
    Reg reg;
    while ((reg = find_degree_less(num_colors)) != NoReg) {
      colors_.set(reg, NoColor);
      g_.remove(reg);
    }
    if ((reg = pick()) == NoReg) {
      break;
    }
    colors_.set(reg, NoColor);
    g_.remove(reg);
  }
}

RegAllocator::Reg RegAllocator::find_degree_less(Color degree) const noexcept {
  for (size_t i = 0, n = g_.size(); i < n; i++) {
    Graph::Degree deg_i = g_.degree(i);
    if (deg_i > 0 && deg_i < Graph::Degree(degree)) {
      return Reg(i);
    }
  }
  return NoReg;
}

RegAllocator::Reg RegAllocator::pick() const noexcept {
  for (size_t i = 0, n = g_.size(); i < n; i++) {
    if (g_.degree(i) != 0) {
      return Reg(i);
    }
  }
  return NoReg;
}

} // namespace onejit

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

RegAllocator::RegAllocator() noexcept : g_{}, g2_{}, stack_{}, colors_{} {
}

RegAllocator::RegAllocator(size_t num_regs) noexcept //
    : g_{num_regs}, g2_{num_regs}, stack_{num_regs}, colors_{num_regs} {
}

RegAllocator::~RegAllocator() noexcept {
}

bool RegAllocator::reset(size_t num_regs) noexcept {
  return g_.reset(num_regs) && g2_.reset(num_regs) //
         && stack_.resize(num_regs) && colors_.resize(num_regs);
}

void RegAllocator::allocate_regs(Color num_colors) noexcept {
  init();
  for (;;) {
    Reg reg;
    while ((reg = find_degree_less_than(num_colors)) != NoReg) {
      stack_.append(Pair{reg, NoColor}); // cannot fail
      g_.remove(reg);
    }
    if ((reg = pick()) == NoReg) {
      break;
    }
    stack_.append(Pair{reg, NoColor}); // cannot fail
    g_.remove(reg);
  }
  assign_colors();
}

void RegAllocator::init() noexcept {
  stack_.clear();
  for (Reg reg = 0, nreg = g_.size(); reg < nreg; ++reg) {
    // remove self-connections, they change regs degree
    g_.set(reg, reg, false);
  }
  g2_.dup(g_); // cannot fail
}

RegAllocator::Reg RegAllocator::find_degree_less_than(Degree degree) const noexcept {
  for (size_t i = 0, n = g_.size(); i < n; i++) {
    Degree deg_i = g_.degree(i);
    if (deg_i > 0 && deg_i < Degree(degree)) {
      return Reg(i);
    }
  }
  return NoReg;
}

// pick a register in g_ to be spilled. currently picks the register with highest degree
RegAllocator::Reg RegAllocator::pick() const noexcept {
  Reg reg = NoReg;
  Degree deg = 0;
  for (size_t i = 0, n = g_.size(); i < n; i++) {
    Degree deg_i = g_.degree(i);
    if (deg_i > deg) {
      reg = Reg(i);
      deg = deg_i;
    }
  }
  return reg;
}

void RegAllocator::assign_colors() noexcept {
  const size_t n = stack_.size();
  colors_.resize(n); // cannot fail

  for (size_t i = n; i != 0; i--) {
    Pair pair = stack_[i - 1];
    Reg reg = pair.reg;

    // initialize the list of available colors
    colors_.fill(true);

    // add back reg i.e. check which neighbors have been added already
    // and connect reg to them
    Graph::Node neighbor = 0;
    while ((neighbor = g2_.first_set(reg, neighbor)) != Graph::NoPos) {
      // if neighbor is already in g_, reconnect it to reg
      // and mark its color as occupied
      if (g_.degree(neighbor) != 0) {
        g_.set(reg, neighbor, true);
        colors_.set(stack_[neighbor].color, false);
      }
    }
    // add self-connection: needed to guarantee degree(reg) != 0
    // which is used in the while() above to check whether reg is present in g_
    g_.set(reg, reg, true);

    // use lowest available color. it may be >= num_colors i.e. spilled
    pair.color = colors_.first_set();
    stack_.set(i - 1, pair);
  }
}

} // namespace onejit

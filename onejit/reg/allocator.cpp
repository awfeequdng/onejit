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

#include <onejit/reg/allocator.hpp>

namespace onejit {
namespace reg {

Allocator::Allocator() noexcept : g_{}, g2_{}, stack_{}, colors_{} {
}

Allocator::Allocator(Size num_regs) noexcept         //
    : g_{num_regs}, g2_{num_regs}, stack_{num_regs}, //
      hints_{}, colors_{num_regs}, avail_colors_{num_regs} {
  hints_.reserve(num_regs);
}

Allocator::~Allocator() noexcept {
}

bool Allocator::reset(Size num_regs) noexcept {
  hints_.clear();
  return g_.reset(num_regs) && g2_.reset(num_regs)              //
         && stack_.resize(num_regs) && hints_.reserve(num_regs) //
         && colors_.resize(num_regs) && avail_colors_.resize(num_regs);
}

void Allocator::add_hint(Reg reg, Color color) noexcept {
  if (!hints_) {
    hints_.resize(size()); // cannot fail
    hints_.fill(NoColor);
  }
  hints_.set(reg, color);
}

void Allocator::allocate_regs(Color num_colors) noexcept {
  init();
  for (;;) {
    Reg reg;
    while ((reg = find_degree_less_than(num_colors)) != NoReg) {
      stack_.append(reg); // cannot fail
      g_.remove(reg);
    }
    if ((reg = pick()) == NoReg) {
      break;
    }
    stack_.append(reg); // cannot fail
    g_.remove(reg);
  }
  assign_colors(num_colors);
}

void Allocator::init() noexcept {
  stack_.clear();
  for (Reg reg = 0, n = size(); reg < n; ++reg) {
    // add self-connections: needed to have g_.degree(reg) != 0
    // even after removing all other regs from g_
    g_.set(reg, reg, true);
    colors_.set(reg, NoColor);
  }
  g2_.dup(g_); // cannot fail
}

Reg Allocator::find_degree_less_than(Degree degree) const noexcept {
  for (Reg i = 0, n = size(); i < n; i++) {
    Degree deg_i = g_.degree(i);
    // deg_i is increased by 2 due to self-connections
    if (deg_i >= 2 && deg_i < 2 + Degree(degree)) {
      return Reg(i);
    }
  }
  return NoReg;
}

// pick a register in g_ to be spilled. currently picks the register with highest degree
Reg Allocator::pick() const noexcept {
  Reg reg = NoReg;
  Degree deg = 0;
  for (Reg i = 0, n = size(); i < n; i++) {
    Degree deg_i = g_.degree(i);
    if (deg_i > deg) {
      reg = i;
      deg = deg_i;
    }
  }
  return reg;
}

void Allocator::assign_colors(Color num_colors) noexcept {
  for (Size n = stack_.size(), i = n; i != 0; i--) {
    Reg reg = stack_[i - 1];

    // initialize the list of available colors
    avail_colors_.fill(true);

    // add back reg i.e. check which neighbors have been added already
    // and connect reg to them
    Reg neighbor = 0;
    while ((neighbor = g2_.first_set(reg, neighbor)) != NoReg) {
      // if neighbor is already in g_, reconnect it to reg
      // and mark its color as occupied
      if (g_.degree(neighbor) != 0) {
        g_.set(reg, neighbor, true);
        avail_colors_.set(colors_[neighbor], false);
      }
      ++neighbor;
    }
    // add self-connection: needed to guarantee degree(reg) != 0
    // which is used in the while() above to check whether reg is present in g_
    g_.set(reg, reg, true);

    // use lowest available color. it may be >= num_colors i.e. spilled
    Color color = avail_colors_.find(true);
    if (hints_) {
      Color alt_color = try_satisfy_hints(reg);
      if (alt_color != NoColor && (alt_color < num_colors || color >= num_colors)) {
        color = alt_color;
      }
    }
    colors_.set(reg, color);
  }
}

Color Allocator::try_satisfy_hints(Reg reg) noexcept {
  Color hint_color = hints_[reg];
  // if a hint for this reg is present, try to honor it
  if (hint_color != NoColor && avail_colors_[hint_color]) {
    return hint_color;
  }

  // if a hint for a connected reg is present, try not to clobber it
  Reg neighbor = 0;
  while ((neighbor = g2_.first_set(reg, neighbor)) != NoReg) {
    if (colors_[neighbor] == NoColor) {
      Color hint_neighbor_color = hints_[neighbor];
      if (hint_neighbor_color != NoColor) {
        avail_colors_.set(hint_neighbor_color, false);
      }
    }
    ++neighbor;
  }
  return avail_colors_.find(true);
}

} // namespace reg
} // namespace onejit

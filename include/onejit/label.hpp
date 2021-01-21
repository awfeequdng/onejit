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
 * label.hpp
 *
 *  Created on Jan 21, 2020
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_LABEL_HPP
#define ONEJIT_LABEL_HPP

#include <onejit/node.hpp>

namespace onejit {

////////////////////////////////////////////////////////////////////////////////
// jump destination. it contains an uint32_t that linker will set to
// destination address, relative to Asm base
class Label : public Node {
  using Base = Node;
  friend class Node;
  friend class Func;

public:
  /**
   * construct an invalid Label.
   * exists only to allow placing Label in containers
   * and similar uses that require a default constructor.
   *
   * to create a valid Label, use Func::new_label()
   */
  constexpr Label() noexcept : Base{NodeHeader{LABEL, Bad, 0}, 0, nullptr} {
  }

  constexpr uint16_t index() const noexcept {
    return Base::op();
  }

private:
  // downcast Node to Label
  constexpr explicit Label(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t == LABEL;
  }

  static Label create(uint16_t index, Code *holder) noexcept;
};

std::ostream &operator<<(std::ostream &out, const Label &l);

} // namespace onejit

#endif // ONEJIT_LABEL_HPP

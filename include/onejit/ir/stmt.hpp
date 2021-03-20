/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * stmt.hpp
 *
 *  Created on Jan 18, 2021
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_IR_STMT_HPP
#define ONEJIT_IR_STMT_HPP

#include <onejit/ir/node.hpp>
#include <onejit/opstmt.hpp>

namespace onejit {
namespace ir {

// base class of Stmt0, Stmt1, Stmt2, Stmt3, Stmt4, StmtN
class Stmt : public Node {
  using Base = Node;
  friend class Node;
  friend class Stmt0;

public:
  constexpr Stmt() noexcept : Base{} {
  }

  static constexpr Kind kind() noexcept {
    return Void;
  }

protected:
  // downcast Node to Stmt
  constexpr explicit Stmt(const Node &node) noexcept : Base{node} {
  }

  // downcast helper
  static constexpr bool is_allowed_type(Type t) noexcept {
    return t >= STMT_0 && t <= STMT_N;
  }
};

} // namespace ir
} // namespace onejit

#endif // ONEJIT_IR_STMT_HPP

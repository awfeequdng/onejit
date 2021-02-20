/*
 * onejit - in-memory assembler
 *
 * Copyright (C) 2021 Massimiliano Ghilardi
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
 * compiler.cpp
 *
 *  Created on Feb 19, 2021
 *      Author Massimiliano Ghilardi
 */

#include <onejit/compiler.hpp>
#include <onejit/func.hpp>
#include <onejit/node/call.hpp>
#include <onejit/node/expr.hpp>
#include <onejit/node/stmt1.hpp>
#include <onejit/node/stmt2.hpp>
#include <onejit/node/stmtn.hpp>
#include <onejit/x64/compiler.hpp>

namespace onejit {

Compiler &Compiler::x64(Func &func, Opt flags) noexcept {
  compile(func, flags);
  if (*this && error_.empty()) {
    // pass our internal buffers node_ and error_ to x64::Compiler
    onejit::x64::Compiler{}.compile(func, node_, error_, flags);
  }
  return *this;
}

namespace x64 {

Compiler::operator bool() const noexcept {
  return good_ && func_ && *func_;
}

Compiler &Compiler::compile(Func &func, Vector<Node> &node, Vector<Error> &error,
                            Opt flags) noexcept {
  node.clear();
  func_ = &func;
  node_ = &node;
  error_ = &error;
  flags_ = flags;
  good_ = bool(func);

  if (Node node = func.get_compiled()) {
    return compile(node).finish();
  }
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

Compiler &Compiler::compile(Node node) noexcept {
  const Type t = node.type();
  switch (t) {
  case STMT_0:
    return error(node, "unexpected Stmt0");
  case STMT_1:
    return compile(node.is<Stmt1>());
  case STMT_2:
    return compile(node.is<Stmt2>());
  case STMT_3:
    return error(node, "unexpected Stmt3");
  case STMT_4:
    return error(node, "unexpected Stmt4");
  case STMT_N:
    return compile(node.is<StmtN>());
  default:
    if (Expr expr = node.is<Expr>()) {
      return compile(expr);
    }
    return error(node, "unexpected Node");
  }
}

// ===============================  compile(Expr)  =============================

Compiler &Compiler::compile(Expr expr) noexcept {
  return add(simplify(expr));
}

Expr Compiler::simplify(Expr expr) noexcept {
  return expr; // TODO
}

// ===============================  compile(Stmt1)  ============================

Compiler &Compiler::compile(Stmt1 st) noexcept {
  OpStmt1 op = st.op();
  switch (op) {
  case GOTO:
    op = X86_JMP;
    break;
  case INC:
    op = X86_INC;
    break;
  case DEC:
    op = X86_DEC;
    break;
  case ASM_JA:
    op = X86_JA;
    break;
  case ASM_JAE:
    op = X86_JAE;
    break;
  case ASM_JB:
    op = X86_JB;
    break;
  case ASM_JBE:
    op = X86_JBE;
    break;
  case ASM_JE:
    op = X86_JE;
    break;
  case ASM_JG:
    op = X86_JG;
    break;
  case ASM_JGE:
    op = X86_JGE;
    break;
  case ASM_JL:
    op = X86_JL;
    break;
  case ASM_JLE:
    op = X86_JLE;
    break;
  case ASM_JNE:
    op = X86_JNE;
    break;
  default:
    return error(st, "unexpected Stmt1 operation");
  }
  Expr expr = simplify(st.arg());
  return add(Stmt1{*func_, expr, op});
}

// ===============================  compile(Stmt2)  ============================

Compiler &Compiler::compile(Stmt2 st) noexcept {
  switch (st.op()) {
  case JUMP_IF:
    return compile(st.is<JumpIf>());
  default:
    if (Assign assign = st.is<Assign>()) {
      return compile(assign);
    }
    return error(st, "unexpected Stmt2");
  }
}

// ===============================  compile(StmtN)  ============================

Compiler &Compiler::compile(StmtN st) noexcept {
  switch (st.op()) {
  case ASSIGN_CALL:
    return compile(st.is<AssignCall>());
  case RETURN:
    return compile(st.is<Return>());
  default:
    return error(st, "unexpected StmtN");
  }
}

Compiler &Compiler::compile(AssignCall st) noexcept {
  return add(st); // TODO?
}

Compiler &Compiler::compile(Return st) noexcept {
  return add(st); // TODO?
}

////////////////////////////////////////////////////////////////////////////////

Var Compiler::to_var(Node node) noexcept {
  Expr e = node.is<Expr>();
  Var v = node.is<Var>();
  if (e && !v) {
    // copy Expr result to a Var
    v = Var{*func_, e.kind()};
    compile(Assign{*func_, ASSIGN, v, e});
  }
  return v;
}

Compiler &Compiler::add(const Node &node) noexcept {
  if (node != VoidConst) {
    good_ = good_ && node_ && node_->append(node);
  }
  return *this;
}

Compiler &Compiler::error(const Node &where, Chars msg) noexcept {
  good_ = good_ && error_ && error_->append(Error{where, msg});
  return *this;
}

} // namespace x64
} // namespace onejit

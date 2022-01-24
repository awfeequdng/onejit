/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * assembler.hpp
 *
 *  Created on Jan 23, 2022
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_MIR_ASSEMBLER_HPP
#define ONEJIT_MIR_ASSEMBLER_HPP

#include <onejit/error.hpp>
#include <onejit/fwd.hpp>
#include <onestl/array.hpp>
#include <onestl/chars.hpp>
#include <onestl/crange.hpp>

struct MIR_context;
struct MIR_item;
struct MIR_module;
typedef struct MIR_context *MIR_context_t;
typedef struct MIR_item *MIR_item_t;
typedef struct MIR_module *MIR_module_t;

namespace onejit {
namespace mir {

class Assembler {

public:
  Assembler();

  ~Assembler() noexcept;

  // assemble a function. must be already compiled to MIR instructions.
  Assembler &add(const Func &func) noexcept;

  // return current assembler errors
  constexpr CRange<Error> errors() const noexcept {
    return CRange<Error>{&error_};
  }

  // add an assembler error
  Assembler &error(Node where, Chars msg) noexcept;

  // add an out of memory error
  Assembler &out_of_memory(Node where) noexcept;

private:
  // assemble a MIR instruction.
  Assembler &add(Node node) noexcept;

private:
  MIR_context_t mctx_;
  MIR_module_t mmod_;
  MIR_item_t mfunc_;
  const Func *func_;
  Array<Error> error_;
  bool good_;

}; // class Assembler

} // namespace mir
} // namespace onejit

#endif // ONEJIT_MIR_ASSEMBLER_HPP

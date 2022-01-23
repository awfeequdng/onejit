/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * assembler.cpp
 *
 *  Created on Jan 23, 2022
 *      Author Massimiliano Ghilardi
 */

#include <onejit/mir/assembler.hpp>
#include <onejit_config.h>

#if defined(HAVE_MIR_H)
#include <mir.h>
#elif defined(HAVE_MIR_MIR_H)
#include <mir/mir.h>
#endif

#if defined(HAVE_MIR_GEN_H)
#include <mir-gen.h>
#elif defined(HAVE_MIR_MIR_GEN_H)
#include <mir/mir-gen.h>
#endif

namespace onejit {
namespace mir {

Assembler::Assembler() : ctx_{nullptr}, error_{}, good_{true} {
#ifdef HAVE_MIR
  ctx_ = MIR_init();
  if (!ctx_) {
    error(Node{}, "MIR_init() failed");
  }
#else
  error(Node{}, "MIR support not compiled");
#endif
}

Assembler::~Assembler() noexcept {
#ifdef HAVE_MIR
  if (ctx_) {
    MIR_finish(ctx_);
  }
#endif
}

Assembler &Assembler::error(Node where, Chars msg) noexcept {
  good_ = good_ && error_.append(Error{where, msg});
  return *this;
}

Assembler &Assembler::out_of_memory(Node where) noexcept {
  // always set good_ to false
  good_ = good_ && error_.append(Error{where, "out of memory"}) && false;
  return *this;
}

} // namespace mir
} // namespace onejit

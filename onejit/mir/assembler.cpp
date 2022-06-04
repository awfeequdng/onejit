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

#include <onejit/func.hpp>
#include <onejit/mir/assembler.hpp>
#include <onejit_config.h>
#include <onestl/fmt.hpp>
#include <onestl/writer_cstdio.hpp> // for debugging only
#include <onestl/writer_string.hpp>

#include <cstdio> // for debugging only

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

Assembler::Assembler()
    : mctx_{}, mmod_{}, mfunc_{}, func_{}, param_names_{}, error_{}, good_{true} {
#ifdef HAVE_MIR
  mctx_ = MIR_init();
  if (mctx_) {
    MIR_gen_init(mctx_, 1 /*threads*/);
    MIR_gen_set_optimize_level(mctx_, 0 /*thread*/, 3 /*-O3*/);
    mmod_ = MIR_new_module(mctx_, "m");
    if (!mmod_) {
      error(Node{}, "MIR_new_module() failed");
    }
  } else {
    error(Node{}, "MIR_init() failed");
  }
#else
  error(Node{}, "MIR support not compiled");
#endif
}

Assembler::~Assembler() noexcept {
#ifdef HAVE_MIR
  if (mfunc_) {
    mfunc_ = nullptr;
    MIR_finish_func(mctx_);
  }
  if (mmod_) {
    mmod_ = nullptr;
    MIR_finish_module(mctx_);
  }
  if (mctx_) {
    MIR_finish(mctx_);
    mctx_ = nullptr;
  }
#endif
}

#ifdef HAVE_MIR
static MIR_type_t to_mir(Kind kind) noexcept {
  static const MIR_type_t mtypes[] = {
      MIR_T_UNDEF, MIR_T_UNDEF, MIR_T_U8,             // eBad, eVoid, eBool
      MIR_T_I8,    MIR_T_I16,   MIR_T_I32, MIR_T_I64, // eInt*
      MIR_T_U8,    MIR_T_U16,   MIR_T_U32, MIR_T_U64, // eUint*
      MIR_T_UNDEF, MIR_T_F,     MIR_T_D,   MIR_T_LD,  // eFloat*
      MIR_T_U64,                                      // ePtr
  };
  return mtypes[kind.nosimd().val()];
}

static MIR_type_t to_mir_param(Kind kind) noexcept {
  static const MIR_type_t mtypes[] = {
      MIR_T_UNDEF, MIR_T_UNDEF, MIR_T_I64,            // eBad, eVoid, eBool
      MIR_T_I64,   MIR_T_I64,   MIR_T_I64, MIR_T_I64, // eInt*
      MIR_T_I64,   MIR_T_I64,   MIR_T_I64, MIR_T_I64, // eUint*
      MIR_T_UNDEF, MIR_T_F,     MIR_T_D,   MIR_T_LD,  // eFloat*
      MIR_T_I64,                                      // ePtr
  };
  return mtypes[kind.nosimd().val()];
}

#endif // HAVE_MIR

Assembler &Assembler::add(const Func &func) noexcept {
  func_ = &func;
#ifdef HAVE_MIR
  Vars params = func.params();
  Vars results = func.results();
  size_t param_n = params.size();
  size_t result_n = results.size();
  Array<MIR_var_t> mparams(param_n);
  Array<MIR_type_t> mresults(result_n);

  param_names_.resize(param_n);
  mparams.resize(param_n);

  for (size_t i = 0; i < param_n; i++) {
    Fmt{&param_names_[i]} << "arg" << (i + 1);

    mparams.set(i, MIR_var_t{to_mir_param(params[i].kind()), param_names_[i].c_str(), 0});
  }
  for (size_t i = 0; i < result_n; i++) {
    mresults.set(i, to_mir(results[i].kind()));
  }

  String name(func.name().chars()); // must be '\0' terminated
  mfunc_ = MIR_new_func_arr(mctx_, name.c_str(), mresults.size(), mresults.data(), //
                            mparams.size(), mparams.data());

  Node body = func.get_compiled(MIR);
  if (body) {
    add(body);
  } else {
    error(Node{}, "function is not compiled for MIR arch, cannot assemble it");
  }
  MIR_finish_func(mctx_);
  mfunc_ = nullptr;

#else
  (void)mctx_;
  (void)mmod_;
  (void)mfunc_;
#endif
  return *this;
}

Assembler &Assembler::add(Node node) noexcept {
#ifdef HAVE_MIR
  Fmt{stdout} << node << '\n';
#else
  (void)node;
#endif
  return *this;
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

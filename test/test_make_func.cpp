/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * test_make_func.cpp
 *
 *  Created on Jun 25, 2021
 *      Author Massimiliano Ghilardi
 */

#include "test.hpp"

#include <onejit/func.hpp>
#include <onejit/ir.hpp>

namespace onejit {

Func &Test::make_func_fib(Kind kind) {
  Func &f = func.reset(&holder, Name{&holder, "fib"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Const one = One(f, kind);
  Const two = Two(f, kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t fib(uint64_t n) {
   *   if (n > 2) {
   *     return f(n-1) + f(n-2);
   *   } else {
   *     return 1;
   *   }
   * }
   */

  f.set_body(                                                           //
      If{f, Binary{f, GTR, n, two},                                     //
         Return{f,                                                      //
                Tuple{f,                                                //
                      ADD,                                              //
                      Call{f, f.fheader(), {Binary{f, SUB, n, one}}},   //
                      Call{f, f.fheader(), {Binary{f, SUB, n, two}}}}}, //
         Return{f, one}});                                              //
  return f;
}

Func &Test::make_func_loop(Kind kind) {
  Func &f = func.reset(&holder, Name{&holder, "loop"}, FuncType{&holder, {kind}, {kind}});
  Var n = f.param(0);
  Var total = f.result(0);
  Var i{f, kind};
  Const zero = Zero(kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * uint64_t loop(uint64_t n) {
   *   uint64_t total = 0, i;
   *   for (i = 0; i < n; i++) {
   *     total += i;
   *   }
   *   return total;
   * }
   */

  f.set_body( //
      Block{f,
            {Assign{f, ASSIGN, total, zero},
             For{
                 f,                              //
                 Assign{f, ASSIGN, i, zero},     // init
                 Binary{f, LSS, i, n},           // test
                 Inc{f, i},                      // post
                 Assign{f, ADD_ASSIGN, total, i} // body
             },
             Return{f, total}}});
  return f;
}

Func &Test::make_func_memchr(Kind kind) {
  Func &f = func.reset(&holder, Name{&holder, "memchr"}, //
                       FuncType{&holder, {Ptr, kind, Uint8}, {Ptr}});
  Var addr = f.param(0);
  Var len = f.param(1);
  Var ch = f.param(2);
  Var i{f, kind};
  Const zero = Zero(kind);

  /**
   * jit equivalent of C/C++ source code
   *
   * const uint8* memchr(const uint8_t* addr, uint64_t len, uint8_t ch) {
   *   uint64_t i;
   *   for (i = 0; i < len; i++) {
   *     if (addr[i] == ch) {
   *       return addr + i;
   *     }
   *   }
   *   return NULL;
   * }
   */

  f.set_body( //
      Block{
          f,
          {For{
               f,                          //
               Assign{f, ASSIGN, i, zero}, // init
               Binary{f, LSS, i, len},     // test
               Inc{f, i},                  // post
               /**/                        // body
               If{f, Binary{f, EQL, Mem{f, Uint8, {addr, i}}, ch},
                  Return{f, Tuple{f, ADD, addr, i}}},
           },
           Return{f, Zero(Ptr)}} // namespace onejit
      });

  return f;
}

} // namespace onejit

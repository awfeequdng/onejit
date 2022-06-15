/*
 * onejit - JIT compiler in C++
 *
 * Copyright (C) 2018-2021 Massimiliano Ghilardi
 *
 *     This Source Code Form is subject to the terms of the Mozilla Public
 *     License, v. 2.0. If a copy of the MPL was not distributed with this
 *     file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * hash_fwd.hpp
 *
 *  Created on Jun 04, 2022
 *      Author Massimiliano Ghilardi
 */

#ifndef ONEJIT_HASH_FWD_HPP
#define ONEJIT_HASH_FWD_HPP

#include <functional>

#ifdef __ANDROID__
/* needed by termux */
#define NAMESPACE_NDK ::__ndk1
#define NAMESPACE_NDK_BEGIN inline namespace __ndk1 {
#define NAMESPACE_NDK_END }
#else
#define NAMESPACE_NDK
#define NAMESPACE_NDK_BEGIN
#define NAMESPACE_NDK_END
#endif

namespace std {
NAMESPACE_NDK_BEGIN

template <class T> struct hash;

NAMESPACE_NDK_END
}

#endif // ONEJIT_HASH_FWD_HPP


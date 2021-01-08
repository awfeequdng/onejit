#ifndef gomacro_asm_template_h
#define gomacro_asm_template_h

/*
 * gomacro - A Go interpreter with Lisp-like macros
 *
 * Copyright (C) 2018 Massimiliano Ghilardi
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
 * asm_template.h
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define Z (ints + 81)
#define A (ints + 82)
#define B (ints + 83)

#define z(l) (*(i##l *)Z)
#define a(l) (*(i##l *)A)
#define b(l) (*(i##l *)B)

#define uz(l) (*(u##l *)Z)
#define ua(l) (*(u##l *)A)
#define ub(l) (*(u##l *)B)

#if defined(__amd64) || defined(__amd64__) || defined(__i386) || \
    defined(__i386__)
i64 _(i64 ax);
#else
#define _(arg) arg
#endif

#endif /* gomacro_asm_template_h */

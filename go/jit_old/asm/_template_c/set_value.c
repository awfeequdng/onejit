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
 * set_value.c
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

#include "asm_template.h"

void ZeroInt8(u64 *ints) { z(8) = 0; }
void ZeroInt16(u64 *ints) { z(16) = 0; }
void ZeroInt32(u64 *ints) { z(32) = 0; }
void ZeroInt64(u64 *ints) { z(64) = 0; }

void SetInt8(u64 *ints) { z(8) = 0x55; }
void SetInt16(u64 *ints) { z(16) = 0x5566; }
void SetInt32(u64 *ints) { z(32) = 0x55667788; }
void SetInt64b(u64 *ints) { z(64) = 0x55; }
void SetInt64l(u64 *ints) { z(64) = 0x55667788; }
void SetInt64q(u64 *ints) { z(64) = 0x5566778899aabbcc; }

void SetUint8(u64 *ints) { uz(8) = 0x55; }
void SetUint16(u64 *ints) { uz(16) = 0x5566; }
void SetUint32(u64 *ints) { uz(32) = 0x55667788; }
void SetUint64b(u64 *ints) { uz(64) = 0x55; }
void SetUint64l(u64 *ints) { uz(64) = 0x55667788; }
void SetUint64q(u64 *ints) { uz(64) = 0x5566778899aabbcc; }

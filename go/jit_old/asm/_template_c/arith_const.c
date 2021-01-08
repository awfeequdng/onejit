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
 * binary_arith_const.c
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

#include "asm_template.h"

void IncInt8c(u64 *ints) { z(8) += 0x55; }
void IncInt16c(u64 *ints) { z(16) += 0x5566; }
void IncInt32c(u64 *ints) { z(32) += 0x55667788; }
void IncInt64bc(u64 *ints) { z(64) += 0x55; }
void IncInt64lc(u64 *ints) { z(64) += 0x55667788; }
void IncInt64qc(u64 *ints) { z(64) += 0x5566778899aabbcc; }

void AddInt8c(u64 *ints) { z(8) = a(8) + 0x55; }
void AddInt16c(u64 *ints) { z(16) = a(16) + 0x5566; }
void AddInt32c(u64 *ints) { z(32) = a(32) + 0x55667788; }
void AddInt64bc(u64 *ints) { z(64) = a(64) + 0x55; }
void AddInt64lc(u64 *ints) { z(64) = a(64) + 0x55667788; }
void AddInt64qc(u64 *ints) { z(64) = a(64) + 0x5566778899aabbcc; }

void DecInt8c(u64 *ints) { z(8) -= 0x55; }
void DecInt16c(u64 *ints) { z(16) -= 0x5566; }
void DecInt32c(u64 *ints) { z(32) -= 0x55667788; }
void DecInt64bc(u64 *ints) { z(64) -= 0x55; }
void DecInt64lc(u64 *ints) { z(64) -= 0x55667788; }
void DecInt64qc(u64 *ints) { z(64) -= 0x5566778899aabbcc; }

void SubInt8c(u64 *ints) { z(8) = a(8) - 0x55; }
void SubInt16c(u64 *ints) { z(16) = a(16) - 0x5566; }
void SubInt32c(u64 *ints) { z(32) = a(32) - 0x55667788; }
void SubInt64bc(u64 *ints) { z(64) = a(64) - 0x55; }
void SubInt64lc(u64 *ints) { z(64) = a(64) - 0x55667788; }
void SubInt64qc(u64 *ints) { z(64) = a(64) - 0x5566778899aabbcc; }

void MulInt8c(u64 *ints) { z(8) = a(8) * 0x55; }
void MulInt16c(u64 *ints) { z(16) = a(16) * 0x5566; }
void MulInt32c(u64 *ints) { z(32) = a(32) * 0x55667788; }
void MulInt64bc(u64 *ints) { z(64) = a(64) * 0x55; }
void MulInt64lc(u64 *ints) { z(64) = a(64) * 0x55667788; }
void MulInt64qc(u64 *ints) { z(64) = a(64) * 0x5566778899aabbcc; }

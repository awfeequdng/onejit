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
 * bitwise.c
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

#include "asm_template.h"

i64 And_l_ax(u64 *ints, i64 ax) { return _(ax) & 0x55667788; }
i64 And_q_ax(u64 *ints, i64 ax) { return _(ax) & 0x5566778899aabbccll; }
i64 And_ax(u64 *ints, i64 ax) { return _(ax) & a(64); }

i64 Or_l_ax(u64 *ints, i64 ax) { return _(ax) | 0x55667788; }
i64 Or_q_ax(u64 *ints, i64 ax) { return _(ax) | 0x5566778899aabbccll; }
i64 Or_ax(u64 *ints, i64 ax) { return _(ax) | a(64); }

i64 Xor_l_ax(u64 *ints, i64 ax) { return _(ax) ^ 0x55667788; }
i64 Xor_q_ax(u64 *ints, i64 ax) { return _(ax) ^ 0x5566778899aabbccll; }
i64 Xor_ax(u64 *ints, i64 ax) { return _(ax) ^ a(64); }

i64 Andnot_l_ax(u64 *ints, i64 ax) { return _(ax) & ~0x55667788; }
i64 Andnot_q_ax(u64 *ints, i64 ax) { return _(ax) & ~0x5566778899aabbccll; }
i64 Andnot_ax(u64 *ints, i64 ax) { return _(ax) & ~a(64); }

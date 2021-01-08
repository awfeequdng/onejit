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
 * arith.c
 *
 *  Created on May 20, 2018
 *      Author Massimiliano Ghilardi
 */

#include "asm_template.h"

i64 Add_l(i64 ax) { return _(ax) + 0x55667788; }
i64 Add_q(i64 ax) { return _(ax) + 0x5566778899aabbcc; }
i64 Add(i64 ax) { return _(ax) + a(64); }

i64 Sub_l(i64 ax) { return _(ax) - 0x55667788; }
i64 Sub_q(i64 ax) { return _(ax) - 0x5566778899aabbcc; }
i64 Sub(i64 ax) { return _(ax) - a(64); }

i64 Mul_l(i64 ax) { return _(ax) * 0x55667788; }
i64 Mul_q(i64 ax) { return _(ax) * 0x5566778899aabbcc; }
i64 Mul(i64 ax) { return _(ax) * a(64); }

i64 Quo_l(i64 ax) { return _(ax) / 0x55667788; }
i64 Quo_q(i64 ax) { return _(ax) / 0x5566778899aabbcc; }
i64 Quo(i64 ax) { return _(ax) / a(64); }

u64 QuoU(u64 ax) { return _(ax) / ua(64); }

i64 Rem(i64 ax) { return _(ax) % a(64); }
u64 RemU(u64 ax) { return _(ax) % ua(64); }

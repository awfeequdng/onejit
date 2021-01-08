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
 * _extend.c
 *
 *  Created on Feb 08, 2019
 *      Author Massimiliano Ghilardi
 */

#include <stdint.h>

void extend8_16(uint8_t a, uint16_t *b) { *b = a; }

void extend8_32(uint8_t a, uint32_t *b) { *b = a; }

void extend8_64(uint8_t a, uint64_t *b) { *b = a; }

void extend16_32(uint16_t a, uint32_t *b) { *b = a; }

void extend16_64(uint16_t a, uint64_t *b) { *b = a; }

void extend32_64(uint32_t a, uint64_t *b) { *b = a; }

void sextend8_16(int8_t a, int16_t *b) { *b = a; }

void sextend8_32(int8_t a, int32_t *b) { *b = a; }

void sextend8_64(int8_t a, int64_t *b) { *b = a; }

void sextend16_32(int16_t a, int32_t *b) { *b = a; }

void sextend16_64(int16_t a, int64_t *b) { *b = a; }

void sextend32_64(int32_t a, int64_t *b) { *b = a; }
